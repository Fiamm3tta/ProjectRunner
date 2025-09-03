// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ProjectRunnerSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FMapClearRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bClearedNormal = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bClearedHard = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BestTimeNormal = FLT_MAX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BestTimeHard = FLT_MAX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOptionA = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOptionB = false;
};

UCLASS()
class PROJECTRUNNER_API UProjectRunnerSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FMapClearRecord> MapRecords;
};
