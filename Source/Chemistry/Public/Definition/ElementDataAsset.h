// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ElementDataAsset.generated.h"

/**
 * Element Data to define different kinds of energy types and used in Reactions
 */
UCLASS(Const)
class CHEMISTRY_API UElementDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="ElementProperties")
	FName ElementName;

	UPROPERTY(EditDefaultsOnly, Category = "ElementProperties")
	float InitialEnergy;

public:
	
	bool operator==(UElementDataAsset& Rhs);
};
