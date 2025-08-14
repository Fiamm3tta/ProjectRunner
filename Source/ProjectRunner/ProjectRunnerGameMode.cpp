// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectRunnerGameMode.h"
#include "ProjectRunnerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectRunnerGameMode::AProjectRunnerGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
