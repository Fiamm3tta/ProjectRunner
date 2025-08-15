// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrapBase.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class PROJECTRUNNER_API ATrapBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrapBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trap")
	UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trap")
	UStaticMeshComponent* Mesh;

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComp,
					   AActor* OtherActor,
					   UPrimitiveComponent* OtherComp,
					   int32 OtherBodyIndex,
					   bool bFromSweep,
					   const FHitResult& SweepResult);

	bool IsPlayer(AActor* Other) const;
	void Kill(AActor* Other) const;
};
