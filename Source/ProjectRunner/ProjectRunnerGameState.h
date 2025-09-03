// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ProjectRunnerGameState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRUNNER_API AProjectRunnerGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AProjectRunnerGameState();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Run")
	void StartRun();

	UFUNCTION(BlueprintCallable, Category = "Run")
	void StopRun();

	UFUNCTION(BlueprintCallable, Category = "Run")
	float GetElapsed() const { return ElapsedTime; }

	UFUNCTION(BlueprintCallable, Category = "Options")
	void SetOptionA(bool bOn) { bOptionA = bOn; }

	UFUNCTION(BlueprintCallable, Category = "Options")
	void SetOptionB(bool bOn) { bOptionB = bOn; }

	UFUNCTION(BlueprintCallable, Category = "Options")
	bool GetOptionA() const { return bOptionA; }

	UFUNCTION(BlueprintCallable, Category = "Options")
	bool GetOptionB() const { return bOptionB; }

private:
	bool bRunActive = false;
	float ElapsedTime = 0.f;
	bool bOptionA = false;
	bool bOptionB = false;
};
