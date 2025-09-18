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
#if WITH_EDITOR
    if (!IsRunningGame()) { Super::Shutdown(); return; }
#endif

    SaveToDist();
    Super::Shutdown();
}

void UProjectRunnerGameInstance::StartRun(FName LevelName)
{
    CurrentLevel = LevelName;
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
        Rec.BestTimeNormal = FMath::Min(Rec.BestTimeNormal, (float)Elapsed);
        Rec.bOptionA = Rec.bOptionA || bOptionA;
        Rec.bOptionB = Rec.bOptionB || bOptionB;
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

bool UProjectRunnerGameInstance::SaveToDist()
{
#if WITH_EDITOR
    if (!IsRunningGame()) return false;
#endif

    EnsureSaveCache();
    if(!SaveCache) return false;

    UProjectRunnerSaveGame* Snapshot = DuplicateObject<UProjectRunnerSaveGame>(SaveCache, this);
    UProjectRunnerSaveGame* ToSave = Snapshot ? Snapshot : SaveCache.Get();

    return UGameplayStatics::SaveGameToSlot(ToSave, SLOT_NAME, USER_INDEX);
}

void UProjectRunnerGameInstance::EnsureSaveCache()
{
    if (!IsValid(SaveCache))
    {
        SaveCache = Cast<UProjectRunnerSaveGame>(
            UGameplayStatics::CreateSaveGameObject(UProjectRunnerSaveGame::StaticClass()));
    }
}