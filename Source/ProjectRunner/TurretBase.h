// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurretBase.generated.h"

class UStaticMeshComponent;
class USceneComponent;

UCLASS()
class PROJECTRUNNER_API ATurretBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurretBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectRunnerProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float FireRange = 100.f;

	UPROPERTY()
	APawn* PlayerPawn = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float FireRate = 2.f;
	FTimerHandle FireRateTimerHandle;

	void RotateTurret(FVector LookAtTarget);
	bool InFireRange();
	void CheckFireCondition();
	void Fire();
};
