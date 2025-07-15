// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Definition/ElementDataAsset.h"
#include "Definition/ChemicalMaterialDataAsset.h"
#include "Definition/ReactionDataAsset.h"
#include "Definition/CatalystDataAsset.h"
#include "ChemistrySubsystem.generated.h"

/**
 * Chemistry portion of the elements plugin
 */
UCLASS()
class CHEMISTRY_API UChemistrySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	// Static function to manage the creation of the UChemistrySubsystem
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnElementsSubsystemCreated, class UChemistrySubsystem*);
protected:
	static FOnElementsSubsystemCreated OnElementsSubsystemCreatedDelegate;
public:
	static FDelegateHandle RegisterOnElementsSubsystemCreated(FOnElementsSubsystemCreated::FDelegate&& Delegate);

private:
	TArray<const UElementDataAsset*> Elements;
	TArray<const UChemicalMaterialDataAsset*> Materials;
	TArray<const UCatalystDataAsset*> Catalysts;
	TArray<const UReactionDataAsset*> Reactions;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection);

	void AddChemicalMaterial(const UChemicalMaterialDataAsset* ChemicalMaterialData);
	void AddElement(const class UElementDataAsset* ElementData);
	void AddReaction(const class UReactionDataAsset* ReactionData);
	void AddCatalyst(const class UCatalystDataAsset* CatalystData);
	// void AddExternalModifier()
};
