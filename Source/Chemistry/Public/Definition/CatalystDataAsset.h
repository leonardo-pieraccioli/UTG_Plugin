// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CatalystDataAsset.generated.h"

/**
 * Entity used to reduce reaction activation threshold. 
 */
UCLASS(Const)
class CHEMISTRY_API UCatalystDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FName CatalystName;

	UPROPERTY(EditDefaultsOnly)
	float ActivationThresholdModifier;

	bool operator=(UCatalystDataAsset& Rhs);
};
