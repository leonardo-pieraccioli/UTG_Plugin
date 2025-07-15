// Fill out your copyright notice in the Description page of Project Settings.


#include "ChemistrySubsystem.h"
#include "ElementsDeveloperSettings.h"
#include "ChemistryLog.h"

UChemistrySubsystem::FOnElementsSubsystemCreated UChemistrySubsystem::OnElementsSubsystemCreatedDelegate;

FDelegateHandle UChemistrySubsystem::RegisterOnElementsSubsystemCreated(FOnElementsSubsystemCreated::FDelegate&& Delegate)
{
	for (TObjectIterator<UChemistrySubsystem> ElementsIt; ElementsIt; ++ElementsIt)
	{
		if (UChemistrySubsystem* ElementsSubsystem = *ElementsIt; ElementsSubsystem->GetOuter()->IsA(APlayerController::StaticClass()))
		{
			Delegate.Execute(ElementsSubsystem);
		}
	}
	// Register for hint manager created in the future
	return OnElementsSubsystemCreatedDelegate.Add(MoveTemp(Delegate));
}

void UChemistrySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	const UElementsDeveloperSettings* DevSettings = GetDefault<UElementsDeveloperSettings>();
	
	// Initialize entities definitions
	for (const UElementDataAsset* Element : DevSettings->GetElements())
	{
		AddElement(Element);
	}
	for (const UReactionDataAsset* Reaction : DevSettings->GetReactions())
	{
		AddReaction(Reaction);
	}	
	for (const UCatalystDataAsset* Catalyst : DevSettings->GetCatalysts())
	{
		AddCatalyst(Catalyst);
	}	
	for (const UChemicalMaterialDataAsset* ChemicalMaterial : DevSettings->GetChemicalMaterials())
	{
		AddChemicalMaterial(ChemicalMaterial);
	}
}

//void UChemistrySubsystem::AddChemicalMaterial(const UChemistryMaterialDataAsset* ChemistryMaterialData) { }

void UChemistrySubsystem::AddChemicalMaterial(const UChemicalMaterialDataAsset* ChemicalMaterialData)
{
	check(ChemicalMaterialData);

	if (Materials.Contains(ChemicalMaterialData))
	{
		UE_LOG(LogElementsChemistry, Error, TEXT("Material %s already declared!"), *ChemicalMaterialData->MaterialName.ToString());
		return;
	}

	Materials.Add(ChemicalMaterialData);
	UE_LOG(LogElementsChemistry, Display, TEXT("Material %s added to subsystem!"), *ChemicalMaterialData->MaterialName.ToString());
}

void UChemistrySubsystem::AddElement(const UElementDataAsset* ElementData)
{
	check(ElementData);

	if (Elements.Contains(ElementData))
	{
		UE_LOG(LogElementsChemistry, Error, TEXT("Element %s already declared!"), *ElementData->ElementName.ToString());
		return;
	}

	Elements.Add(ElementData);
	UE_LOG(LogElementsChemistry, Display, TEXT("Element %s added to subsystem!"), *ElementData->ElementName.ToString());
}

void UChemistrySubsystem::AddReaction(const UReactionDataAsset* ReactionData)
{
	check(ReactionData);

	// Should define what it means two have two equal reactions. For now only equal DataAsset generate errors
	if (Reactions.Contains(ReactionData))
	{
		UE_LOG(LogElementsChemistry, Error, TEXT("Reaction %s already declared!"), *ReactionData->GetFName().ToString());
		return;
	}

	// TODO: change to new data structure to optimize search
	Reactions.Add(ReactionData);
	UE_LOG(LogElementsChemistry, Display, TEXT("Reaction %s added to subsystem!"), *ReactionData->GetFName().ToString());
}

void UChemistrySubsystem::AddCatalyst(const UCatalystDataAsset* CatalystData)
{
	check(CatalystData);

	if (Catalysts.Contains(CatalystData))
	{
		UE_LOG(LogElementsChemistry, Error, TEXT("Catalyst %s already declared!"), *CatalystData->CatalystName.ToString());
		return;
	}

	Catalysts.Add(CatalystData);
	UE_LOG(LogElementsChemistry, Display, TEXT("Catalyst %s added to subsystem!"), *CatalystData->CatalystName.ToString());
}
