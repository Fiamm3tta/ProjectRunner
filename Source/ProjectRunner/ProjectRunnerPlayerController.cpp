// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProjectRunnerPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AProjectRunnerPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);

		UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	}

	FString MapName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
	if(MapName == "MainMenuMap")
	{
		ShowScreen(EUIScreen::MainMenu);
	}
	else
	{
		ShowScreen(EUIScreen::HUD);
	}
}

void AProjectRunnerPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
    {
        // ESC → TogglePauseMenu
        if (PauseAction)
        {
            EIC->BindAction(PauseAction, ETriggerEvent::Started, this, &AProjectRunnerPlayerController::TogglePauseMenu);
        }
    }
}

void AProjectRunnerPlayerController::ShowScreen(EUIScreen Screen)
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}

	if (TSubclassOf<UUserWidget>* WidgetClass = ScreenClasses.Find(Screen))
	{
		CurrentWidget = CreateWidget<UUserWidget>(this, *WidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
			CurrentScreen = Screen;

			if (Screen == EUIScreen::HUD)
			{
				FInputModeGameOnly Mode;
				SetInputMode(Mode);
				bShowMouseCursor = false;
				SetPause(false);

				SetIgnoreLookInput(false);
				SetIgnoreMoveInput(false);
			}
			else 
			{
				FInputModeGameAndUI Mode;
				Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				SetInputMode(Mode);
				bShowMouseCursor = true;

				const bool bShouldPause = (Screen == EUIScreen::Pause || Screen == EUIScreen::Result || Screen == EUIScreen::Death);
        		SetPause(bShouldPause);

				SetIgnoreLookInput(true);
        		SetIgnoreMoveInput(true);
			}
		}
	}
}

void AProjectRunnerPlayerController::HideScreen()
{
	if(CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
	SetPause(false);
}

void AProjectRunnerPlayerController::TogglePauseMenu()
{
	if(CurrentScreen == EUIScreen::Pause)
	{
		ShowScreen(EUIScreen::HUD);
	}
	else if (CurrentScreen == EUIScreen::HUD)
	{
		ShowScreen(EUIScreen::Pause);
	}
}