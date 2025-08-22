// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectRunnerGameMode.generated.h"

UCLASS(minimalapi)
class AProjectRunnerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectRunnerGameMode();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void HandleLevelClear();
};



