// Copyright Epic Games, Inc. All Rights Reserved.


#include "UTG_ElementsPluginPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "UTG_ElementsPluginCameraManager.h"

AUTG_ElementsPluginPlayerController::AUTG_ElementsPluginPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = AUTG_ElementsPluginCameraManager::StaticClass();
}

void AUTG_ElementsPluginPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}
