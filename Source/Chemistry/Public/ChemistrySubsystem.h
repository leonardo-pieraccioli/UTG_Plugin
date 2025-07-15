// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Definition/ChemistryMaterialDataAsset.h"
#include "Definition/ElementDataAsset.h"
#include "ChemistrySubsystem.generated.h"

/**
 * 
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
	TSet<const UElementDataAsset*> Elements;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection);

	void AddChemicalMaterial(const class UChemistryMaterialDataAsset* ChemistryMaterialData);
	void AddElement(const class UElementDataAsset* ElementData);
	// void AddReaction()
	// void AddCatalyst()
	// void AddExternalModifier()
};
