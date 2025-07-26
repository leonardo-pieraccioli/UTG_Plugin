// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "Definition/ElementDataAsset.h"
#include "Definition/ChemicalElement.h"

#include "Definition/ChemicalMaterialDataAsset.h"
#include "Definition/ChemicalMaterial.h"

#include "Definition/ReactionDataAsset.h"
#include "Definition/ChemicalReaction.h"

#include "Definition/CatalystDataAsset.h"
#include "Definition/ChemicalCatalyst.h"

#include "ChemistrySubsystem.generated.h"

/**
 * Chemistry portion of the elements plugin
 */
UCLASS()
class CHEMISTRY_API UChemistrySubsystem : public UTickableWorldSubsystem
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
	// TArray<const UElementDataAsset*> Elements;
	// TArray<const UChemicalMaterialDataAsset*> Materials;
	// TArray<const UCatalystDataAsset*> Catalysts;
	// TArray<const UReactionDataAsset*> Reactions;
	
	TMap<FName, FChemicalElement> RuntimeElements;
	TMap<FName, FChemicalMaterial> RuntimeMaterials;
	TMap<FName, FChemicalCatalyst> RuntimeCatalysts;
	TMap<FName, FChemicalReaction> RuntimeReactions;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection);
	virtual void Deinitialize() override { Super::Deinitialize(); }

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
	FChemicalReaction& ShouldStartReaction(FName ReactionName);
	FChemicalReaction RecognizeReactionPattern(FGuid ProximityGroupId);

	TMap<FGuid, FChemicalReaction> ActiveReactions; // List of active reactions in the world
	
	// TODO: implement this using a graph structure to allow to end proximity
	TMap<FGuid, TMap<FGuid, FChemicalMaterial*>> EntitiesInProximity;

public:
	FChemicalElement* GetElement(FName ElementName);
	FChemicalMaterial* GetMaterial(FName MaterialName);
	FChemicalCatalyst* GetCatalyst(FName CatalystName);
	// FChemicalReaction* GetReaction(FName ReactionName); // Not sure if we need this
	
	// Return a copy of an existing material or catalyst starting from runtime structures
	UFUNCTION(BlueprintCallable, Category = "Chemistry")	
	FChemicalMaterial GenerateMaterial(FName MaterialName, bool& MaterialFound);

	//TODO: refactor this to use a more generic approach for proximity detection
	UFUNCTION(BlueprintCallable, Category = "Chemistry", meta = (DisplayName = "Begin Proximity"))
	bool BeginProximity(UPARAM(ref) FChemicalMaterial& Material1, UPARAM(ref) FChemicalMaterial& Material2);

	UFUNCTION(BlueprintCallable, Category = "Chemistry", meta = (DisplayName = "End Proximity"))
	bool EndProximity(UPARAM(ref) FChemicalMaterial& Material1, UPARAM(ref) FChemicalMaterial& Material2);

	UFUNCTION(BlueprintCallable, Category = "Chemistry", meta = (DisplayName = "Check Proximity"))
	bool CheckProximity(UPARAM(ref) FChemicalMaterial& Material1, UPARAM(ref) FChemicalMaterial& Material2);

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return true; }

	virtual void Tick(float DeltaTime) override;
	
	virtual bool IsTickable() const override { return GetWorld() != nullptr; }

	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UChemistrySubsystem, STATGROUP_Tickables); }
};
