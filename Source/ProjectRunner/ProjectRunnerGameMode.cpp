// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectRunnerGameMode.h"
#include "ProjectRunnerCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectRunnerPlayerController.h"

AProjectRunnerGameMode::AProjectRunnerGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	GameStateClass = AProjectRunnerGameState::StaticClass();
}

void AProjectRunnerGameMode::BeginPlay()
{
	Super::BeginPlay();

	GS = GetGameState<AProjectRunnerGameState>();
	if(GS) GS->StartRun();

	GI = GetGameInstance<UProjectRunnerGameInstance>();
	FName CurrentLevel(*UGameplayStatics::GetCurrentLevelName(this, true));
	if(GI) GI->StartRun(CurrentLevel, bHard);
}

void AProjectRunnerGameMode::HandleLevelClear()
{	
	UE_LOG(LogTemp, Log, TEXT("Level Cleared!"));

	// 클리어 기록(걸린 시간, 추가 미션 성공 여부) 세이브 파일에 저장
	if (GS)
	{
		CheckTimeLimit();
		CheckKillCount();
	}
	if (GI && GS) GI->FinishRun(GS->GetElapsed(), GS->GetOptionA(), GS->GetOptionB());
	if (GS) GS->StopRun();

	// 결과 화면 (ShowScreen에 플레이어 무력화(?) 코드 있음)
	AProjectRunnerPlayerController* PC = Cast<AProjectRunnerPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC) PC->ShowScreen(EUIScreen::Result);
	else UE_LOG(LogTemp, Warning, TEXT("GameMode: PlayerController not found"));
}

void AProjectRunnerGameMode::CheckTimeLimit()
{
	GS->SetOptionA(TargetTime >= GS->GetElapsed());
}

void AProjectRunnerGameMode::CheckKillCount()
{
	GS->SetOptionB(TargetKillCount <= KillCount);
}

void AProjectRunnerGameMode::AddKill()
{
	KillCount++;
}