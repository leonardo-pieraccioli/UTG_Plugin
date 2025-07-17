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

USTRUCT(BlueprintType)
struct FChemicalCatalyst
{
	GENERATED_BODY()
public:
	FChemicalCatalyst() : ID(FGuid::NewGuid()) {};
	FChemicalCatalyst(const UCatalystDataAsset* Catalyst) : Type(Catalyst->CatalystName), ID(FGuid::NewGuid()), ActivationThresholdModifier(Catalyst->ActivationThresholdModifier) {};
	FChemicalCatalyst(const FChemicalCatalyst& Other) : Type(Other.Type), ID(FGuid::NewGuid()), ActivationThresholdModifier(Other.ActivationThresholdModifier) {};
	FChemicalCatalyst(FChemicalCatalyst&& Other) noexcept : Type(MoveTemp(Other.Type)), ID(MoveTemp(Other.ID)), ActivationThresholdModifier(Other.ActivationThresholdModifier) {};
	FChemicalCatalyst& operator=(const FChemicalCatalyst& Rhs)
	{
		if (this != &Rhs)
		{
			Type = Rhs.Type;
			ID = MoveTemp(Rhs.ID);
			ActivationThresholdModifier = Rhs.ActivationThresholdModifier;
		}
		return *this;
	};
	FChemicalCatalyst& operator=(FChemicalCatalyst&& Rhs) noexcept
	{
		if (this != &Rhs)
		{
			Type = MoveTemp(Rhs.Type);
			ID = MoveTemp(Rhs.ID);
			ActivationThresholdModifier = MoveTemp(Rhs.ActivationThresholdModifier);
		}
		return *this;
	};

private:
	FName Type;
	FGuid ID;
	float ActivationThresholdModifier;

public:
	FName GetType();
	float GetActivationThresholdModifier();
	// ? IsIstantaneous ?
};
