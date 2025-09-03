// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectRunnerGameInstance.h"
#include "Kismet/GameplayStatics.h"

static constexpr TCHAR SLOT_NAME[] = TEXT("ProjectRunnerSave");
static constexpr int32 USER_INDEX = 0;

void UProjectRunnerGameInstance::Init()
{
    Super::Init();
    LoadOrCreateSave();
}

void UProjectRunnerGameInstance::Shutdown()
{
    SaveToDist();
    Super::Shutdown();
}

void UProjectRunnerGameInstance::StartRun(FName LevelName, bool bHard)
{
    CurrentLevel = LevelName;
    bCurrentHard = bHard;
    RunStartSec = FPlatformTime::Seconds();
}

void UProjectRunnerGameInstance::FinishRun(float ClearTimeSeconds, bool bOptionA, bool bOptionB)
{
    if(!SaveCache || CurrentLevel.IsNone())
    {
        CurrentLevel = NAME_None;
        return;
    }

    const double Elapsed = (ClearTimeSeconds > 0.f) ? ClearTimeSeconds : (FPlatformTime::Seconds() - RunStartSec);

    FMapClearRecord& Rec = SaveCache->MapRecords.FindOrAdd(CurrentLevel);
    if(bCurrentHard)
    {
        Rec.bClearedHard = true;
        Rec.BestTimeHard = FMath::Min(Rec.BestTimeHard, (float)Elapsed);
    }
    else
    {
        Rec.bClearedNormal = true;
        Rec.BestTimeNormal = FMath::Min(Rec.BestTimeHard, (float)Elapsed);
        Rec.bOptionA = Rec.bOptionA || bOptionA;
        Rec.bOptionB = Rec.bOptionB || bOptionA;
    }

    CurrentLevel = NAME_None;
}

FMapClearRecord UProjectRunnerGameInstance::GetRecord(FName LevelName) const
{
    if(SaveCache)
    {
        if(FMapClearRecord* Found = SaveCache->MapRecords.Find(LevelName))
        {
            return *Found;
        }
    }

    return FMapClearRecord{};
}

void UProjectRunnerGameInstance::LoadOrCreateSave()
{
    // Load
    if(UGameplayStatics::DoesSaveGameExist(SLOT_NAME, USER_INDEX))
    {
        SaveCache = Cast<UProjectRunnerSaveGame>(UGameplayStatics::LoadGameFromSlot(SLOT_NAME, USER_INDEX));
    }

    // Create
    if(!SaveCache)
    {
        SaveCache = Cast<UProjectRunnerSaveGame>(UGameplayStatics::CreateSaveGameObject(UProjectRunnerSaveGame::StaticClass()));
    }
}

bool UProjectRunnerGameInstance::SaveToDist() const
{
    if(!SaveCache) return false;
    return UGameplayStatics::SaveGameToSlot(SaveCache, SLOT_NAME, USER_INDEX);
}