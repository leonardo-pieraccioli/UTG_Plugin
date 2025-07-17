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

// TODO: Add initialization of Runtime data structures
void UChemistrySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	const UElementsDeveloperSettings* DevSettings = GetDefault<UElementsDeveloperSettings>();
	
	// Initialize entities definitions
	for (const UElementDataAsset* Element : DevSettings->GetElements())
	{
		AddElement(Element);
	}
	for (const UCatalystDataAsset* Catalyst : DevSettings->GetCatalysts())
	{
		AddCatalyst(Catalyst);
	}	
	for (const UChemicalMaterialDataAsset* ChemicalMaterial : DevSettings->GetChemicalMaterials())
	{
		AddChemicalMaterial(ChemicalMaterial);
	}
	for (const UReactionDataAsset* Reaction : DevSettings->GetReactions())
	{
		AddReaction(Reaction);
	}
}

void UChemistrySubsystem::AddElement(const UElementDataAsset* ElementData)
{
	check(ElementData);

	if (RuntimeElements.Contains(ElementData->ElementName))
	{
		UE_LOG(LogElementsChemistry, Error, TEXT("Element %s already declared!"), *ElementData->ElementName.ToString());
		return;
	}

	// Elements.Add(ElementData);

	RuntimeElements.Emplace(ElementData->ElementName, ElementData);
	UE_LOG(LogElementsChemistry, Display, TEXT("Element %s added to subsystem!"), *ElementData->ElementName.ToString());
}

void UChemistrySubsystem::AddCatalyst(const UCatalystDataAsset* CatalystData)
{
	check(CatalystData);

	if (RuntimeCatalysts.Contains(CatalystData->CatalystName))
	{
		UE_LOG(LogElementsChemistry, Error, TEXT("Catalyst %s already declared!"), *CatalystData->CatalystName.ToString());
		return;
	}

	// Catalysts.Add(CatalystData);
	RuntimeCatalysts.Emplace(CatalystData->CatalystName, CatalystData);
	UE_LOG(LogElementsChemistry, Display, TEXT("Catalyst %s added to subsystem!"), *CatalystData->CatalystName.ToString());
}

void UChemistrySubsystem::AddChemicalMaterial(const UChemicalMaterialDataAsset* ChemicalMaterialData)
{
	check(ChemicalMaterialData);

	if (RuntimeMaterials.Contains(ChemicalMaterialData->MaterialName))
	{
		UE_LOG(LogElementsChemistry, Error, TEXT("Material %s already declared!"), *ChemicalMaterialData->MaterialName.ToString());
		return;
	}

	// Materials.Add(ChemicalMaterialData);
	RuntimeMaterials.Emplace(ChemicalMaterialData->MaterialName, CreateMaterialFromData(ChemicalMaterialData));
	UE_LOG(LogElementsChemistry, Display, TEXT("Material %s added to subsystem!"), *ChemicalMaterialData->MaterialName.ToString());
}

void UChemistrySubsystem::AddReaction(const UReactionDataAsset* ReactionData)
{
	check(ReactionData);

	// Check if there is another reaction
	// Should define what it means two have two equal reactions. For now only equal DataAsset generate errors
	// TODO: change to new data structure to optimize search
	
	RuntimeReactions.Emplace(ReactionData->GetFName(), CreateReactionFromData(ReactionData));
	UE_LOG(LogElementsChemistry, Display, TEXT("Reaction %s added to subsystem!"), *ReactionData->GetFName().ToString());
}

FChemicalMaterial UChemistrySubsystem::CreateMaterialFromData(const UChemicalMaterialDataAsset* MaterialData)
{
	check(MaterialData);

	FChemicalMaterial NewMaterial{ MaterialData };
	for (auto ElementsData = MaterialData->InitialElements; UElementDataAsset* Element : ElementsData)
	{
		NewMaterial.AttachElement(*GetElement(Element->ElementName));
	}

	return NewMaterial;
}

FChemicalReaction UChemistrySubsystem::CreateReactionFromData(const UReactionDataAsset* ReactionData)
{
	check(ReactionData);

	FChemicalReaction NewReaction{ ReactionData };
	for (auto ElementsData = ReactionData->Products.Elements; FElementReactionTuple ElementTuple : ElementsData)
	{
		NewReaction.Products.Elements.Emplace(ElementTuple.Coefficient, GetElement(ElementTuple.Element->ElementName));
		UE_LOG(LogElementsChemistry, Display, TEXT("Element %s with coefficient %f added to Products of Reaction %s"), *ElementTuple.Element->ElementName.ToString(), ElementTuple.Coefficient, *ReactionData->GetFName().ToString());
	}

	for (auto CatalystsData = ReactionData->Products.Catalysts; FCatalystReactionTuple CatalystTuple : CatalystsData)
	{
		NewReaction.Products.Catalysts.Emplace(CatalystTuple.Coefficient, GetCatalyst(CatalystTuple.Catalyst->CatalystName));
		UE_LOG(LogElementsChemistry, Display, TEXT("Catalyst %s with coefficient %f added to Products of Reaction %s"), *CatalystTuple.Catalyst->CatalystName.ToString(), CatalystTuple.Coefficient, *ReactionData->GetFName().ToString());
	}

	for (auto MaterialsData = ReactionData->Products.Materials; FMaterialReactionTuple MaterialTuple : MaterialsData)
	{
		NewReaction.Products.Materials.Emplace(MaterialTuple.Coefficient, GetMaterial(MaterialTuple.Material->MaterialName));
		UE_LOG(LogElementsChemistry, Display, TEXT("Material %s with coefficient %f added to Products of Reaction %s"), *MaterialTuple.Material->MaterialName.ToString(), MaterialTuple.Coefficient, *ReactionData->GetFName().ToString());
	}

	for (auto CatalystsData = ReactionData->Reagents.Catalysts; FCatalystReactionTuple CatalystTuple : CatalystsData)
	{
		NewReaction.Reagents.Catalysts.Emplace(CatalystTuple.Coefficient, GetCatalyst(CatalystTuple.Catalyst->CatalystName));
		UE_LOG(LogElementsChemistry, Display, TEXT("Catalyst %s with coefficient %f added to Reagents of Reaction %s"), *CatalystTuple.Catalyst->CatalystName.ToString(), CatalystTuple.Coefficient, *ReactionData->GetFName().ToString());
	}

	for (auto MaterialsData = ReactionData->Reagents.Materials; FMaterialReactionTuple MaterialTuple : MaterialsData)
	{
		NewReaction.Reagents.Materials.Emplace(MaterialTuple.Coefficient, GetMaterial(MaterialTuple.Material->MaterialName));
		UE_LOG(LogElementsChemistry, Display, TEXT("Material %s with coefficient %f added to Reagents of Reaction %s"), *MaterialTuple.Material->MaterialName.ToString(), MaterialTuple.Coefficient, *ReactionData->GetFName().ToString());
	}
	UE_LOG(LogElementsChemistry, Display, TEXT("Reaction %s created!"), *ReactionData->GetFName().ToString());
	return NewReaction;
}

FChemicalElement* UChemistrySubsystem::GetElement(FName ElementName)
{
	FChemicalElement* FoundElement = RuntimeElements.Find(ElementName);
	if (FoundElement == nullptr)
	{
		UE_LOG(LogElementsChemistry, Error, TEXT("Try getting Element %s but not found in subsystem!"), *ElementName.ToString());
		return nullptr;
	}

	return FoundElement;
}

FChemicalMaterial* UChemistrySubsystem::GetMaterial(FName MaterialName)
{
	FChemicalMaterial* FoundMaterial = RuntimeMaterials.Find(MaterialName);
	if (FoundMaterial == nullptr)
	{
		UE_LOG(LogElementsChemistry, Error, TEXT("Try getting Material %s but not found in subsystem!"), *MaterialName.ToString());
		return nullptr;
	}
	return FoundMaterial;
}

FChemicalCatalyst* UChemistrySubsystem::GetCatalyst(FName CatalystName)
{
	FChemicalCatalyst* FoundCatalyst = RuntimeCatalysts.Find(CatalystName);
	if (FoundCatalyst == nullptr)
	{
		UE_LOG(LogElementsChemistry, Error, TEXT("Try getting Catalyst %s but not found in subsystem!"), *CatalystName.ToString());
		return nullptr;
	}
	return FoundCatalyst;
}

FChemicalReaction* UChemistrySubsystem::GetReaction(FName ReactionName)
{
	FChemicalReaction* FoundReaction = RuntimeReactions.Find(ReactionName);
	if (FoundReaction == nullptr)
	{
		UE_LOG(LogElementsChemistry, Error, TEXT("Try getting Reaction %s but not found in subsystem!"), *ReactionName.ToString());
		return nullptr;
	}
	return FoundReaction;
}

FChemicalMaterial& UChemistrySubsystem::GenerateMaterial(FName MaterialName, bool& MaterialFound)
{
	FChemicalMaterial* FoundMaterial = GetMaterial(MaterialName);
	if (!FoundMaterial)
	{
		MaterialFound = false;
		UE_LOG(LogElementsChemistry, Error, TEXT("Cannot generate material %s: not found in subsystem"), *MaterialName.ToString());
		return *FoundMaterial;
	}
	MaterialFound = true;
	return *FoundMaterial;
}

FChemicalCatalyst& UChemistrySubsystem::GenerateCatalyst(FName CatalystName, bool& CatalystFound)
{
	FChemicalCatalyst* FoundCatalyst = GetCatalyst(CatalystName);
	if (!FoundCatalyst)
	{
		CatalystFound = false;
		UE_LOG(LogElementsChemistry, Error, TEXT("Cannot generate catalyst %s: not found in subsystem"), *CatalystName.ToString());
		return *FoundCatalyst;
	}
	CatalystFound = true;
	return *FoundCatalyst;
}

FChemicalReaction& UChemistrySubsystem::StartReaction(FName ReactionName)
{
	FChemicalReaction* FoundReaction = GetReaction(ReactionName);
	if (!FoundReaction)
	{
		UE_LOG(LogElementsChemistry, Error, TEXT("Cannot generate reaction %s: not found in subsystem"), *ReactionName.ToString());
		return *FoundReaction;
	}

	// TODO: add to ActiveReactions data structure and adapt return

	return *FoundReaction;
}

