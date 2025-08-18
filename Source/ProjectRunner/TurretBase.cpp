// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "ProjectRunnerProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ATurretBase::ATurretBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret MEsh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

// Called when the game starts or when spawned
void ATurretBase::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATurretBase::CheckFireCondition, FireRate, true);
}

// Called every frame
void ATurretBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(PlayerPawn && InFireRange())
	{
		RotateTurret(PlayerPawn->GetActorLocation());
	}
}

void ATurretBase::RotateTurret(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	TurretMesh->SetWorldRotation(
		FMath::RInterpTo(
			TurretMesh->GetComponentRotation(), 
			LookAtRotation, 
			UGameplayStatics::GetWorldDeltaSeconds(this),
			25.f)
		);
}

void ATurretBase::CheckFireCondition()
{
    if (InFireRange() && PlayerPawn)
    {
        Fire();
    }
}

bool ATurretBase::InFireRange()
{
	if (PlayerPawn)
    {
        float Distance = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());
        if (Distance <= FireRange)
        {
            return true;
        }
    }

    return false;
}

void ATurretBase::Fire()
{
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
	if(ProjectileClass)
	{
		AProjectRunnerProjectile* Projectile = GetWorld()->SpawnActor<AProjectRunnerProjectile>(ProjectileClass, Location, Rotation);
		if (Projectile && Projectile->GetProjectileMovement())
        {
            Projectile->GetProjectileMovement()->SetVelocityInLocalSpace(FVector::ForwardVector * Projectile->GetProjectileMovement()->InitialSpeed);
            Projectile->SetOwner(this);
        }
	}
}

