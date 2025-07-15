// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UTG_ElementsPluginGameMode.generated.h"

/**
 *  Simple GameMode for a first person game
 */
UCLASS(abstract)
class AUTG_ElementsPluginGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUTG_ElementsPluginGameMode();
};



