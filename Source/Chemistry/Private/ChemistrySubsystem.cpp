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

void UChemistrySubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// TODO: -------- REMOVE --------------------------------------------------------------------------------------------------------------------------------
	/**/static float timer = 0;																															 /**/
	/**/timer += DeltaTime;																																 /**/
	/**/if (timer < 2) return;																															 /**/
	/**/UE_LOG(LogElementsChemistry, Display, TEXT("GROUP PROXIMITY REPORT:"));																			 /**/
	/**/for (auto ProxGroup : EntitiesInProximity)																										 /**/
	/**/{																																				 /**/
	/**/	UE_LOG(LogElementsChemistry, Display, TEXT("Proximity group: %s"), *ProxGroup.Key.ToString());												 /**/
	/**/	for (auto ProxElement : ProxGroup.Value)																									 /**/
	/**/	{																																			 /**/
	/**/		UE_LOG(LogElementsChemistry, Display, TEXT("\t%s"), *ProxElement->GetIDString())														 /**/
	/**/	}																																			 /**/
	/**/}																																				 /**/
	/**/timer = 0;																																		 /**/
	// ------------------------------------------------------------------------------------------------------------------------------------------------------

	if (ActiveReactions.IsEmpty())
	{
		return;
	}
	for (auto ReactionTuple : ActiveReactions)
	{
		ReactionTuple.Value.ProcessReaction();
	}
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

	for (auto MaterialsData = ReactionData->Reagents.Materials; auto MaterialProperties : MaterialsData)
	{
		NewReaction.Reagents.Emplace(MaterialProperties.Coefficient, GetMaterial(MaterialProperties.Material->MaterialName), MaterialProperties.ActivationElement->ElementName, MaterialProperties.ActivationThreshold);
		UE_LOG(LogElementsChemistry, Display,
			TEXT("Material %s with coefficient %f added to Reagents of Reaction %s\nActivation Threshold is %s = %f"),
			*MaterialProperties.Material->MaterialName.ToString(),
			MaterialProperties.Coefficient,
			*ReactionData->GetFName().ToString(),
			*MaterialProperties.ActivationElement->ElementName.ToString(),
			MaterialProperties.ActivationThreshold
		);
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

FChemicalMaterial UChemistrySubsystem::GenerateMaterial(FName MaterialName, bool& MaterialFound)
{
	FChemicalMaterial* FoundMaterial = GetMaterial(MaterialName);
	if (!FoundMaterial)
	{
		MaterialFound = false;
		UE_LOG(LogElementsChemistry, Error, TEXT("Cannot generate material %s: not found in subsystem"), *MaterialName.ToString());
		return *FoundMaterial;
	}
	MaterialFound = true;
	FChemicalMaterial NewMaterial{ *FoundMaterial };
	UE_LOG(LogElementsChemistry, Display, TEXT("Material %s generated successfully"), *NewMaterial.ToString());
	return NewMaterial;
}

FChemicalReaction UChemistrySubsystem::RecognizeReactionPattern(FGuid ProximityGroupId = FGuid(0,0,0,0))
{
	FChemicalReaction NewReaction;
	if (ProximityGroupId.IsValid())
	{
		TArray<FChemicalMaterial*> ProximityGroup =	EntitiesInProximity[ProximityGroupId];
		
		for (auto ReactionRow : RuntimeReactions)
		{
			FChemicalReaction Reaction = ReactionRow.Value;
			if (Reaction.CheckReagents(ProximityGroup))
			{
				UE_LOG(LogElementsChemistry, Display, TEXT("REACTION FOUND! %s"), *Reaction.Name.ToString());
			}
		}
	}
	else
	{
		// not implemented
	}
	return NewReaction;
}

bool UChemistrySubsystem::BeginProximity(UPARAM(ref) FChemicalMaterial& Material1, UPARAM(ref) FChemicalMaterial& Material2)
{
	// Same ID -> Same material -> Cannot signal
	if (Material1.GetID() == Material2.GetID())
	{
		UE_LOG(LogElementsChemistry, Warning, TEXT("Signaled proximity between the same material. This should not be possible."));
		return false;
	}

	// Valid IDs, Same proximity IDs -> Proximity already signaled
	if (Material1.ProximityId.IsValid() && Material2.ProximityId.IsValid() && Material1.ProximityId == Material2.ProximityId)
	{
		//UE_LOG(LogElementsChemistry, Warning, TEXT("Materials %s and %s have already signaled proximity, with ID %s"), *Material1.ToString(), *Material2.ToString(), *Material1.ProximityId.ToString());
		return false;
	}

	// Both invalid proximity ID -> New proximity group
	if (!Material1.ProximityId.IsValid() && !Material2.ProximityId.IsValid())
	{
		FGuid NewProximityId = FGuid::NewGuid();
		EntitiesInProximity.Add(NewProximityId);

		EntitiesInProximity[NewProximityId].Add(&Material1);
		Material1.ProximityId = NewProximityId;
		
		EntitiesInProximity[NewProximityId].Add(&Material2);
		Material2.ProximityId = NewProximityId;

		UE_LOG(LogElementsChemistry, Display, TEXT("New proximity group! Materials %s and %s \tProximity ID: %s"), *Material1.ToString(), *Material2.ToString(), *NewProximityId.ToString());
		RecognizeReactionPattern(NewProximityId);
		return true;
	}

	// One of them invalid -> Add it to the other proximity group
	if (Material1.ProximityId.IsValid() && !Material2.ProximityId.IsValid())
	{
		if (!EntitiesInProximity.Contains(Material1.ProximityId))
		{
			UE_LOG(LogElementsChemistry, Error, TEXT("Material %s has ProximityId %s but not found in EntitiesInProximity!"), *Material1.ToString(), *Material1.ProximityId.ToString());
			return false;
		}

		EntitiesInProximity[Material1.ProximityId].Add(&Material2);
		Material2.ProximityId = Material1.ProximityId;
		UE_LOG(LogElementsChemistry, Display, TEXT("Proximity signaled: %s united with %s \tProximity ID: %s"), *Material2.ToString(), *Material1.ToString(), *Material1.ProximityId.ToString());
		RecognizeReactionPattern(Material1.ProximityId);
		return true;
	}
	if (!Material1.ProximityId.IsValid() && Material2.ProximityId.IsValid())
	{
		if (!EntitiesInProximity.Contains(Material2.ProximityId))
		{
			UE_LOG(LogElementsChemistry, Error, TEXT("Material %s has ProximityId %s but not found in EntitiesInProximity!"), *Material2.ToString(), *Material2.ProximityId.ToString());
			return false;
		}

		EntitiesInProximity[Material2.ProximityId].Add(&Material1);
		Material1.ProximityId = Material2.ProximityId;
		UE_LOG(LogElementsChemistry, Display, TEXT("Proximity signaled: %s united with %s \tProximity ID: %s"), *Material1.ToString(), *Material2.ToString(), *Material1.ProximityId.ToString());
		RecognizeReactionPattern(Material2.ProximityId);
		return true;
	}

	// Valid IDs, Different proximity IDs -> Unite proximity groups
	if (Material1.ProximityId.IsValid() && Material2.ProximityId.IsValid() && Material1.ProximityId != Material2.ProximityId)
	{
		int NGroup1 = EntitiesInProximity[Material1.ProximityId].Num();
		int NGroup2 = EntitiesInProximity[Material2.ProximityId].Num();
		FGuid IdTransfer = Material1.ProximityId;
		FGuid IdDestination = Material2.ProximityId;
		if (NGroup1 > NGroup2)
		{
			IdTransfer = Material2.ProximityId;
			IdDestination = Material1.ProximityId;
		}
	
		for (auto Material : EntitiesInProximity[IdTransfer])
		{
			Material->ProximityId = IdDestination;
			EntitiesInProximity[IdDestination].Add(Material);
		}
		EntitiesInProximity.Remove(IdTransfer);
		
        UE_LOG(LogElementsChemistry, Display, TEXT("Proximity groups %s and %s have been united."), *IdTransfer.ToString(), *IdDestination.ToString());
		RecognizeReactionPattern(IdDestination);
		return true;
	}
	
	// Other cases
	UE_LOG(LogElementsChemistry, Error, TEXT("Something went wrong between Materials %s and %s"), *Material1.ToString(), *Material2.ToString());
	return false;
} 

bool UChemistrySubsystem::EndProximity(UPARAM(ref) FChemicalMaterial& Material1, UPARAM(ref) FChemicalMaterial& Material2)
{
	if (!Material1.ProximityId.IsValid() || !Material2.ProximityId.IsValid())
	{
		UE_LOG(LogElementsChemistry, Error, TEXT("Cannot end proximity between Materials %s and %s: one or both have no ProximityId"), *Material1.ToString(), *Material2.ToString());
		return false;
	}

	if (Material1.ProximityId != Material2.ProximityId)
	{
		UE_LOG(LogElementsChemistry, Error, TEXT("Cannot end proximity between Materials %s and %s: they are NOT in the same Proximity Group"), *Material1.ToString(), *Material2.ToString());
		return false;
	}

	FGuid ProximityIdToRemove = Material1.ProximityId;
	if (EntitiesInProximity[ProximityIdToRemove].Remove(&Material1) < 1)
	{
		UE_LOG(LogElementsChemistry, Error, TEXT("Cannot remove Material %s from ProximityId %s cannot be found"), *Material1.ToString(), *Material1.ProximityId.ToString());
		return false;
	}

	if (EntitiesInProximity[ProximityIdToRemove].Remove(&Material2) < 1)
	{
		UE_LOG(LogElementsChemistry, Error, TEXT("Cannot remove Material %s from ProximityId %s cannot be found"), *Material2.ToString(), *Material2.ProximityId.ToString());
		return false;
	}

	if (EntitiesInProximity[ProximityIdToRemove].Num() == 0)
	{
		EntitiesInProximity.Remove(ProximityIdToRemove);
		UE_LOG(LogElementsChemistry, Display, TEXT("No more entities in proximity group %s"), *ProximityIdToRemove.ToString());
	} 

	Material1.ProximityId.Invalidate();
	Material2.ProximityId.Invalidate();

	return true;
}

// TODO: remove
bool UChemistrySubsystem::CheckProximity(UPARAM(ref)FChemicalMaterial& Material1, UPARAM(ref)FChemicalMaterial& Material2)
{
	if (!Material1.ProximityId.IsValid() || !Material2.ProximityId.IsValid())
	{
		UE_LOG(LogElementsChemistry, Error, TEXT("Check proximity: One or more ID invalid"));
		return false;
	}
	
	if (!EntitiesInProximity.Contains(Material1.ProximityId) || !EntitiesInProximity.Contains(Material2.ProximityId))
	{
		UE_LOG(LogElementsChemistry, Error, TEXT("Check proximity: One or more ID not found"));
		return false;
	}

	return Material1.ProximityId == Material2.ProximityId;
}

FChemicalReaction& UChemistrySubsystem::ShouldStartReaction(FName ReactionName)
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

