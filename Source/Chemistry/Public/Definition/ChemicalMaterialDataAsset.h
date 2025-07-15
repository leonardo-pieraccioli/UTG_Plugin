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
	TSet<class UElementDataAsset*> CompatibleElements;

	bool operator==(UChemicalMaterialDataAsset& Rhs);
};
