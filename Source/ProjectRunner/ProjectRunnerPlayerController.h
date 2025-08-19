// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "ProjectRunnerPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;

/**
 *
 */

UENUM(BlueprintType)
enum class EUIScreen : uint8
{
	MainMenu,
	MapSelect,
	HUD,
	Pause,
	Result,
	Death
};

UCLASS()
class PROJECTRUNNER_API AProjectRunnerPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
    UInputAction* PauseAction = nullptr;

	UPROPERTY(EditAnywhere, Category="UI")
    TMap<EUIScreen, TSubclassOf<UUserWidget>> ScreenClasses;

	// Begin Actor interface
protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// End Actor interface

public:
	UFUNCTION(BlueprintCallable) void ShowScreen(EUIScreen Screen);
	UFUNCTION(BlueprintCallable) void HideScreen();
    UFUNCTION(BlueprintCallable) void TogglePauseMenu();

private:
	UPROPERTY() UUserWidget* CurrentWidget = nullptr;
    EUIScreen CurrentScreen = EUIScreen::HUD;
};
