// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectRunnerGameMode.h"
#include "ProjectRunnerCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "ProjectRunnerPlayerController.h"

AProjectRunnerGameMode::AProjectRunnerGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void AProjectRunnerGameMode::HandleLevelClear()
{	
	UE_LOG(LogTemp, Log, TEXT("Level Cleared!"));

	// 결과 화면 (ShowScreen에 플레이어 무력화(?) 코드 있음)
	AProjectRunnerPlayerController* PlayerController = Cast<AProjectRunnerPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController) PlayerController->ShowScreen(EUIScreen::Result);
	else UE_LOG(LogTemp, Warning, TEXT("GameMode: PlayerController not found"));

	// 클리어 기록(걸린 시간, 추가 미션 성공 여부) 세이브 파일에 저장

}