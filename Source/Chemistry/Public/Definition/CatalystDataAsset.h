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
	// Default constructor 
	FChemicalCatalyst() : ID(FGuid::NewGuid()) {};
	// Constructor from UCatalystDataAsset
	FChemicalCatalyst(const UCatalystDataAsset* Catalyst) : 
		Type(Catalyst->CatalystName),
		ID(FGuid::NewGuid()), 
		ActivationThresholdModifier(Catalyst->ActivationThresholdModifier) {};
	// Copy Constructor and Move Constructor
	FChemicalCatalyst(const FChemicalCatalyst& Other) : 
		Type(Other.Type), 
		ID(FGuid::NewGuid()), 
		ActivationThresholdModifier(Other.ActivationThresholdModifier),
		ProximityId(Other.ProximityId) {};
	FChemicalCatalyst(FChemicalCatalyst&& Other) noexcept : 
		Type(MoveTemp(Other.Type)), 
		ID(MoveTemp(Other.ID)), 
		ActivationThresholdModifier(MoveTemp(Other.ActivationThresholdModifier)),
		ProximityId(MoveTemp(Other.ProximityId)) {};
	// Move and Copy Assignment Operators
	FChemicalCatalyst& operator=(const FChemicalCatalyst& Rhs)
	{
		if (this != &Rhs)
		{
			Type = Rhs.Type;
			ID = Rhs.ID;
			ActivationThresholdModifier = Rhs.ActivationThresholdModifier;
			ProximityId = Rhs.ProximityId;
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
			ProximityId = MoveTemp(Rhs.ProximityId);
		}
		return *this;
	};
	// Destructor
	~FChemicalCatalyst() = default;

private:
	FName Type;
	FGuid ID;
	float ActivationThresholdModifier;

public:
	UPROPERTY(BlueprintReadOnly, Category = "Proximity")
	FGuid ProximityId;

	// TODO: these functions are common to every entity, should implement a base class/interface for them
	FName GetType();
	FGuid GetID();
	FString GetIDString() { return *Type.ToString() + FString(" ") + ID.ToString(); };
	FString ToString() { return GetIDString(); };
	// -----------
	
	float GetActivationThresholdModifier();
	// ? IsIstantaneous ?
};
