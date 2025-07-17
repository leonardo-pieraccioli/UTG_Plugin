// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ElementDataAsset.h"
#include "ChemicalMaterialDataAsset.generated.h"


/**
 * Material Data to define entities that can contain different kinds of energy and used in Reactions
 */
UCLASS(Const)
class CHEMISTRY_API UChemicalMaterialDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FName MaterialName;

	UPROPERTY(EditDefaultsOnly)
	TSet<class UElementDataAsset*> InitialElements;

	bool operator==(UChemicalMaterialDataAsset& Rhs);
};

USTRUCT(BlueprintType)
struct FChemicalMaterial
{
	GENERATED_BODY()
public:
	FChemicalMaterial() {};
	FChemicalMaterial(const UChemicalMaterialDataAsset* MaterialData) : Type(MaterialData->MaterialName) {};
	FChemicalMaterial(const FChemicalMaterial& Other) : Type(Other.Type), AttachedElements(Other.AttachedElements), Quantity(Other.Quantity) {};
	FChemicalMaterial(FChemicalMaterial&& Other) noexcept : Type(MoveTemp(Other.Type)), AttachedElements(MoveTemp(Other.AttachedElements)), Quantity(Other.Quantity) {};
	FChemicalMaterial& operator=(const FChemicalMaterial& Rhs)
	{
		if (this != &Rhs)
		{
			Type = Rhs.Type;
			AttachedElements = Rhs.AttachedElements;
			Quantity = Rhs.Quantity;
		}
		return *this;
	}
	FChemicalMaterial& operator=(FChemicalMaterial&& Rhs) noexcept
	{
		if (this != &Rhs)
		{
			Type = MoveTemp(Rhs.Type);
			AttachedElements = MoveTemp(Rhs.AttachedElements);
			Quantity = Rhs.Quantity;
		}
		return *this;
	}

private:
	FName Type;
	TMap<FName, FChemicalElement> AttachedElements;
	float Quantity;

public:
	FName GetType();
	bool AttachElement(FChemicalElement NewElement);
	bool RemoveElement(FName ElementName);
	// ModifyElement();
	// TransformToMaterial();
	float UpdateMass(float Amount);
	float UpdateEnergy(FName ElementName, float Amount, bool& ReturnStatus);
};