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
	UPROPERTY(EditDefaultsOnly, Category="ReactionProperties")
	FReactionReagents Reagents;

	UPROPERTY(EditDefaultsOnly, Category = "ReactionProperties")
	FReactionProducts Products;

	UPROPERTY(EditDefaultsOnly, Category = "ReactionProperties");
	TObjectPtr<UElementDataAsset> RequiredElement;

	UPROPERTY(EditDefaultsOnly, Category = "ReactionProperties");
	float ActivationThreshold;

	UPROPERTY(EditDefaultsOnly, Category = "ReactionProperties")
	int32 Priority{0};
};
