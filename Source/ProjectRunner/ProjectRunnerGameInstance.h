// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProjectRunnerSaveGame.h"
#include "ProjectRunnerGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRUNNER_API UProjectRunnerGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	UFUNCTION(BlueprintCallable)
	void StartRun(FName LevelName, bool bHard);

	UFUNCTION(BlueprintCallable)
	void FinishRun(float ClearTimeSeconds, bool bOptionA, bool bOptionB);

	UFUNCTION(BlueprintPure)
	FMapClearRecord GetRecord(FName LevelName) const;

	UFUNCTION(BlueprintCallable)
	void LoadOrCreateSave();

	UFUNCTION(BlueprintCallable)
	bool SaveToDist() const;

	bool bCurrentHard = false;
private:
	TObjectPtr<UProjectRunnerSaveGame> SaveCache = nullptr;
	FName CurrentLevel = NAME_None;
	double RunStartSec = 0.0;
};
