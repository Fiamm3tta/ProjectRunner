// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectRunnerGameState.h"
#include "ProjectRunnerGameInstance.h"
#include "ProjectRunnerGameMode.generated.h"

UCLASS(minimalapi)
class AProjectRunnerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectRunnerGameMode();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Game")
	void HandleLevelClear();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void CheckTimeLimit();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void CheckKillCount();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void AddKill();

private:
	AProjectRunnerGameState* GS;
	UProjectRunnerGameInstance* GI;

	bool bHard = false;
	float TargetTime = 60.f;
	int32 TargetKillCount = 5;
	int32 KillCount = 0;
};



