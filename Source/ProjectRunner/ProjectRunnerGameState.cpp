// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectRunnerGameState.h"
#include "Kismet/GameplayStatics.h"

AProjectRunnerGameState::AProjectRunnerGameState()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AProjectRunnerGameState::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(!bRunActive)
    {
        return;
    }

    ElapsedTime += DeltaTime;
}

void AProjectRunnerGameState::StartRun()
{
	bRunActive = true;
	ElapsedTime = 0.f;
	bOptionA = false;
	bOptionB = false;
}


void AProjectRunnerGameState::StopRun()
{
    if(!bRunActive) return;
	bRunActive = false;
}