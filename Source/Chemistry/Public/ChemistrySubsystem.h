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

	TMap<FName, FChemicalElement> RuntimeElements;
	TMap<FName, FChemicalMaterial> RuntimeMaterials;
	TMap<FName, FChemicalCatalyst> RuntimeCatalysts;
	TMap<FName, FChemicalReaction> RuntimeReactions;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection);

private:
	void AddChemicalMaterial(const UChemicalMaterialDataAsset* ChemicalMaterialData);
	void AddElement(const class UElementDataAsset* ElementData);
	void AddReaction(const class UReactionDataAsset* ReactionData);
	void AddCatalyst(const class UCatalystDataAsset* CatalystData);
	// void AddExternalModifier()

	FChemicalMaterial CreateMaterialFromData(const UChemicalMaterialDataAsset* MaterialData);
	FChemicalReaction CreateReactionFromData(const UReactionDataAsset* ReactionData);

	FChemicalReaction* GetReaction(FName ReactionName);
	// To generate a new reaction when there components are in proximity
	FChemicalReaction& StartReaction(FName ReactionName);

public:
	FChemicalElement* GetElement(FName ElementName);
	FChemicalMaterial* GetMaterial(FName MaterialName);
	FChemicalCatalyst* GetCatalyst(FName CatalystName);
	// FChemicalReaction* GetReaction(FName ReactionName); // Not sure if we need this
	
// Return a copy of a specific entity
	UFUNCTION(BlueprintCallable, Category = "Chemistry")
	FChemicalMaterial& GenerateMaterial(FName MaterialName, bool& MaterialFound);

	UFUNCTION(BlueprintCallable, Category = "Chemistry")
	FChemicalCatalyst& GenerateCatalyst(FName CatalystName, bool& CatalystFound);

	// Data structure for Proximity
	
	// SignalProximityBegin()
	// SignalProximityEnd()
	
	// Data structure for Active Reactions

	// Tick to update Active Reactions
};
