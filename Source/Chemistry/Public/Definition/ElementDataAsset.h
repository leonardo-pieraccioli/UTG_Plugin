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
	// Default Constructor
	FChemicalElement() : ID(FGuid::NewGuid()) {};
	// Constructor from ElementDataAsset
	FChemicalElement(const UElementDataAsset* Element) : 
		Type(Element->ElementName), 
		ID(FGuid::NewGuid()), 
		Energy(Element->InitialEnergy) {};
	// Copy and Move Constructors
	FChemicalElement(const FChemicalElement& Other) : 
		Type(Other.Type), 
		ID(FGuid::NewGuid()),
		Energy(Other.Energy) {};
	FChemicalElement(FChemicalElement&& Other) noexcept : 
		Type(MoveTemp(Other.Type)), 
		ID(MoveTemp(Other.ID)), 
		Energy(MoveTemp(Other.Energy)) {};
	// Copy and Move Assignment Operators
	FChemicalElement& operator=(const FChemicalElement& Rhs)
	{
		if (this != &Rhs)
		{
			Type = Rhs.Type;
			ID = Rhs.ID;
			Energy = Rhs.Energy;
		}
		return *this;
	};
	FChemicalElement& operator=(FChemicalElement&& Rhs) noexcept
	{
		if (this != &Rhs)
		{
			Type = MoveTemp(Rhs.Type);
			ID = MoveTemp(Rhs.ID);
			Energy = MoveTemp(Rhs.Energy);
		}
		return *this;
	};
	// Destructor
	~FChemicalElement() = default;

private:
	FName Type;
	FGuid ID;
public:
	UPROPERTY(BlueprintReadWrite)
	float Energy;
	FName GetType();
	// useless? 
	float ConsumeEnergy(float Amount);
	float ProvideEnergy(float Amount);
};
