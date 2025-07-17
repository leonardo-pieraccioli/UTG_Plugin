// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ElementDataAsset.h"
#include "ChemicalMaterialDataAsset.h"
#include "CatalystDataAsset.h"
#include "ReactionDataAsset.generated.h"

USTRUCT()
struct FMaterialReactionTuple
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="ReactionProperties");
	float Coefficient;

	UPROPERTY(EditDefaultsOnly, Category = "ReactionProperties");
	TObjectPtr<UChemicalMaterialDataAsset> Material;
};

USTRUCT()
struct FCatalystReactionTuple
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "ReactionProperties");
	float Coefficient;

	UPROPERTY(EditDefaultsOnly, Category = "ReactionProperties");
	TObjectPtr<UCatalystDataAsset> Catalyst;
};

USTRUCT()
struct FElementReactionTuple
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "ReactionProperties");
	float Coefficient;

	UPROPERTY(EditDefaultsOnly, Category = "ReactionProperties");
	TObjectPtr<UElementDataAsset> Element;
};

USTRUCT()
struct FReactionReagents
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "ReactionProperties");
	TArray<FMaterialReactionTuple> Materials;

	UPROPERTY(EditDefaultsOnly, Category = "ReactionProperties");
	TArray<FCatalystReactionTuple> Catalysts;
};

USTRUCT()
struct FReactionProducts
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "ReactionProperties");
	TArray<FMaterialReactionTuple> Materials;

	UPROPERTY(EditDefaultsOnly, Category = "ReactionProperties");
	TArray<FCatalystReactionTuple> Catalysts;

	UPROPERTY(EditDefaultsOnly, Category = "ReactionProperties");
	TArray<FElementReactionTuple> Elements;
};

/**
 * Reactions Data to define reagents and products. Each reactyion has a priority and an activation threshold.
 */
UCLASS(Const)
class CHEMISTRY_API UReactionDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "ReactionProperties");
	FReactionReagents Reagents;

	UPROPERTY(EditDefaultsOnly, Category = "ReactionProperties");
	FReactionProducts Products;

	UPROPERTY(EditDefaultsOnly, Category = "ReactionProperties");
	TObjectPtr<UElementDataAsset> RequiredElement;

	UPROPERTY(EditDefaultsOnly, Category = "ReactionProperties");
	float ActivationThreshold;

	UPROPERTY(EditDefaultsOnly, Category = "ReactionProperties");
	int32 Priority{0};
};

USTRUCT(BlueprintType)
struct FChemicalReaction
{
	GENERATED_BODY()

public:
	FChemicalReaction() = default;
	FChemicalReaction(const UReactionDataAsset* ReactionData);
	FChemicalReaction(const FChemicalReaction& Other) : Products(Other.Products), Reagents(Other.Reagents), RequiredElement(Other.RequiredElement), Priority(Other.Priority), ActivationThreshold(Other.ActivationThreshold) {};
	FChemicalReaction(FChemicalReaction&& Other) noexcept : Products(MoveTemp(Other.Products)), Reagents(MoveTemp(Other.Reagents)), RequiredElement(MoveTemp(Other.RequiredElement)), Priority(MoveTemp(Other.Priority)), ActivationThreshold(MoveTemp(Other.ActivationThreshold)) {};
	FChemicalReaction& operator=(const FChemicalReaction& Rhs)
	{
		if (this != &Rhs)
		{
			Products = Rhs.Products;
			Reagents = Rhs.Reagents;
			RequiredElement = Rhs.RequiredElement;
			Priority = Rhs.Priority;
			ActivationThreshold = Rhs.ActivationThreshold;
		}
		return *this;
	}
	FChemicalReaction& operator=(FChemicalReaction&& Rhs) noexcept
	{
		if (this != &Rhs)
		{
			Products = MoveTemp(Rhs.Products);
			Reagents = MoveTemp(Rhs.Reagents);
			RequiredElement = MoveTemp(Rhs.RequiredElement);
			Priority = MoveTemp(Rhs.Priority);
			ActivationThreshold = MoveTemp(Rhs.ActivationThreshold);
		}
		return *this;
	}

private:
	// TODO: Maybe coefficients are not intuitive, we could use a more descriptive name like "Amount" or "Quantity"
	struct ElementTuple
	{
		float Coefficient;
		FChemicalElement* Element;
	};

	struct CatalystTuple
	{
		float Coefficient;
		FChemicalCatalyst* Catalyst;
	};

	struct MaterialTuple
	{
		float Coefficient;
		FChemicalMaterial* Material;
	};

	struct ProductEntities
	{
		TArray<MaterialTuple> Materials;
		TArray<CatalystTuple> Catalysts;
		TArray<ElementTuple> Elements;
	};

	struct ReagentEntities
	{
		TArray<MaterialTuple> Materials;
		TArray<CatalystTuple> Catalysts;
	};

public:
	ProductEntities Products;
	ReagentEntities Reagents;
	FChemicalElement* RequiredElement; // could be just an FName? maybe a pointer is lighter
	int32 Priority;
	float ActivationThreshold;

	TArray<FName> GetReagentMaterials();
	TArray<FName> GetReagentCatalysts();
	TArray<FName> GetProductMaterials();
	TArray<FName> GetProductCatalysts();
	TArray<FName> GetProductElements();


	// ExecuteReaction();
};