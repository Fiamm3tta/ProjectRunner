// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectRunnerProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectRunnerCharacter.h"
#include "TurretBase.h"

AProjectRunnerProjectile::AProjectRunnerProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectRunnerProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AProjectRunnerProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}

	if(AProjectRunnerCharacter* PlayerCharacter = Cast<AProjectRunnerCharacter>(OtherActor))
	{
		FDamageEvent DamageEvent;
		AController* InstigatorController = GetInstigatorController();
		AActor* DamageCauser = this;
		PlayerCharacter->TakeDamage(DamageAmount, DamageEvent, InstigatorController, DamageCauser);

		Destroy();
	}
	else if(ATurretBase* Turret = Cast<ATurretBase>(OtherActor))
	{
		Turret->Destroy();
		AProjectRunnerCharacter* PC = Cast<AProjectRunnerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		PC->OnEnemyKilled();

		Destroy();
	}
}