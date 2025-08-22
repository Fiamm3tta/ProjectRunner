// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectRunnerCharacter.h"
#include "ProjectRunnerProjectile.h"
#include "ProjectRunnerPlayerController.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AProjectRunnerCharacter

AProjectRunnerCharacter::AProjectRunnerCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void AProjectRunnerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	CurrentWalkSpeed = BaseWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = CurrentWalkSpeed;
}

//////////////////////////////////////////////////////////////////////////// Input

void AProjectRunnerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProjectRunnerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AProjectRunnerCharacter::Look);

		// Dashing
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &AProjectRunnerCharacter::Dash);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AProjectRunnerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AProjectRunnerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AProjectRunnerCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AProjectRunnerCharacter::GetHasRifle()
{
	return bHasRifle;
}

// Health
float AProjectRunnerCharacter::TakeDamage (float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	if(bHitLocked) return 0.f;
	bHitLocked = true;
	GetWorldTimerManager().SetTimer(HitLockoutTimer, this, &AProjectRunnerCharacter::ClearHitLock, HitLockoutSec, false);

	float OldHealth = Health;
	Health = FMath::Clamp(Health - DamageAmount, 0.f, MaxHealth);
	if(Health <= 0.f) Die();

	CurrentWalkSpeed = BaseWalkSpeed;

	return OldHealth - Health;
}

void AProjectRunnerCharacter::ClearHitLock()
{
	bHitLocked = false;
}

void AProjectRunnerCharacter::Die()
{
	AProjectRunnerPlayerController* PlayerController = Cast<AProjectRunnerPlayerController>(GetWorld()->GetFirstPlayerController());
	PlayerController->ShowScreen(EUIScreen::Death);
	UE_LOG(LogTemplateCharacter, Warning, TEXT("Player died"));
}

// Dash
void AProjectRunnerCharacter::Dash()
{
	if(!bCanDash) return;

	FVector Direction = GetLastMovementInputVector();
	if(Direction.IsNearlyZero())
	{
		const FRotator CR = Controller ? Controller->GetControlRotation() : GetActorRotation();
		Direction = FRotationMatrix(CR).GetUnitAxis(EAxis::X);
	}
	Direction.Z = 0.f;
	Direction.Normalize();

	float Strength = DashStrength;
	if(GetCharacterMovement()->IsFalling())
	{
		Strength *= AirDashMultiplier;
	}

	LaunchCharacter(Direction * Strength, true, false);

	bCanDash = false;
	GetWorldTimerManager().SetTimer(DashCooldownHandle, this, &AProjectRunnerCharacter::ResetDash, DashCooldown, false);
}

void AProjectRunnerCharacter::ResetDash()
{
	bCanDash = true;
	UE_LOG(LogTemplateCharacter, Log, TEXT("Dash cooldown ready"));
}

// Speed buff
void AProjectRunnerCharacter::OnEnemyKilled()
{
	CurrentWalkSpeed = FMath::Min(CurrentWalkSpeed + BuffAmount, MaxWalkSpeedCap);
	GetCharacterMovement()->MaxWalkSpeed = CurrentWalkSpeed;
}