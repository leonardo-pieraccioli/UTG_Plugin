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

USTRUCT(BlueprintType)
struct FChemicalElement
{
	GENERATED_BODY()
public:
	FChemicalElement() = default;
	FChemicalElement(const UElementDataAsset* Element) : Type(Element->ElementName), Energy(Element->InitialEnergy) {};
	FChemicalElement(const FChemicalElement& Other) : Type(Other.Type), Energy(Other.Energy) {};
	FChemicalElement(FChemicalElement&& Other) noexcept : Type(MoveTemp(Other.Type)), Energy(MoveTemp(Other.Energy)) {};
	FChemicalElement& operator=(const FChemicalElement& Rhs)
	{
		if (this != &Rhs)
		{
			Type = Rhs.Type;
			Energy = Rhs.Energy;
		}
		return *this;
	};
	FChemicalElement& operator=(FChemicalElement&& Rhs) noexcept
	{
		if (this != &Rhs)
		{
			Type = MoveTemp(Rhs.Type);
			Energy = MoveTemp(Rhs.Energy);
		}
		return *this;
	};

private:
	FName Type;

public:
	UPROPERTY(BlueprintReadWrite)
	float Energy;

public:
	FName GetType();
	// useless? 
	float ConsumeEnergy(float Amount);
	float ProvideEnergy(float Amount);
};
