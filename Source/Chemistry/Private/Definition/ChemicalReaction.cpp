
#include "Definition/ChemicalReaction.h"
#include "ChemistrySubsystem.h"
#include "ChemistryLog.h"

FChemicalReaction::FChemicalReaction(const UReactionDataAsset* ReactionData) : Priority(ReactionData->Priority)
{
	if (!ReactionId.IsValid())
	{
		ReactionId = FGuid::NewGuid();
	}

	Name = ReactionData->GetFName();
	Reagents.Reserve(ReactionData->Reagents.Materials.Num());

	Products.Materials.Reserve(ReactionData->Products.Materials.Num());
	Products.Catalysts.Reserve(ReactionData->Products.Catalysts.Num());
	Products.Elements.Reserve(ReactionData->Products.Elements.Num());
}

FGuid FChemicalReaction::GetId()
{
	return ReactionId;
}

TArray<FName> FChemicalReaction::GetReagentMaterials()
{
	TArray<FName> ReagentMaterialsNames;
	ReagentMaterialsNames.Reserve(Reagents.Num());
	for (FReagentMaterialProperties Material : Reagents)
	{
		ReagentMaterialsNames.Add(Material.Material->GetType());
	}
	return ReagentMaterialsNames;
}

TArray<FName> FChemicalReaction::GetProductMaterials()
{
	TArray<FName> ProductMaterialsNames;
	ProductMaterialsNames.Reserve(Products.Materials.Num());
	for (auto ProductMaterials = Products.Materials; FMaterialTuple Material : ProductMaterials)
	{
		ProductMaterialsNames.Add(Material.Material->GetType());
	}
	return ProductMaterialsNames;
}

TArray<FName> FChemicalReaction::GetProductCatalysts()
{
	TArray<FName> ProductCatalystsNames;
	ProductCatalystsNames.Reserve(Products.Catalysts.Num());
	for (auto ProductCatalysts = Products.Catalysts; FCatalystTuple Catalyst : ProductCatalysts)
	{
		ProductCatalystsNames.Add(Catalyst.Catalyst->GetType());
	}
	return ProductCatalystsNames;
}

TArray<FName> FChemicalReaction::GetProductElements()
{
	TArray<FName> ProductElementsNames;
	ProductElementsNames.Reserve(Products.Elements.Num());
	for (auto ProductElements = Products.Elements; FElementTuple Element : ProductElements)
	{
		ProductElementsNames.Add(Element.Element->GetType());
	}
	return ProductElementsNames;
}

FChemicalReaction FChemicalReaction::ShouldCreateReaction(TArray<FChemicalMaterial*> ProximityGroup)
{
	FChemicalReaction NewReaction;
	TMap<FName, TArray<FChemicalMaterial*>> ParticipantMaterialsByType;
	ParticipantMaterialsByType.Reserve(Reagents.Num());

	// Check if necessary materials are in proximity group
	for (FReagentMaterialProperties MaterialProperty : Reagents)
	{
		FChemicalMaterial* Material = MaterialProperty.Material;
		FChemicalMaterial** FoundParticipantMaterial = ProximityGroup.FindByPredicate([Material](FChemicalMaterial* Other) -> bool { return Material->GetType() == Other->GetType(); });
		if (FoundParticipantMaterial == nullptr)
		{
			return FChemicalReaction();
		}
		FChemicalMaterial* ParticipantMaterial = *FoundParticipantMaterial;
		if (ParticipantMaterialsByType.Find(Material->GetType()) == nullptr)
		{
			ParticipantMaterialsByType.Add(Material->GetType());
		}
		ParticipantMaterialsByType[Material->GetType()].Add(ParticipantMaterial);
	}

	NewReaction.Products = Products;
	NewReaction.Reagents.Reserve(Reagents.Num());

	// Check which material has necessary elements
	for (const auto& MaterialType : ParticipantMaterialsByType)
	{
		for (const auto ParticipantMaterial : MaterialType.Value)
		{
			auto PropertyPredicate = [ParticipantMaterial](FReagentMaterialProperties& Property) -> bool { return Property.Material->GetType() == ParticipantMaterial->GetType(); };
			FReagentMaterialProperties* RequiredProperty = Reagents.FindByPredicate(PropertyPredicate);
			FName RequiredElementName = RequiredProperty->ActivationElement;
			if (ParticipantMaterial->GetElement(RequiredElementName)->Energy >= RequiredProperty->ActivationThreshold)
			{
				RequiredProperty->Material = ParticipantMaterial;
				NewReaction.Reagents.Add(*RequiredProperty);
			}
			else
			{
				return FChemicalReaction();
			}
		}
	}

	NewReaction.ReactionId = FGuid::NewGuid();
	NewReaction.Name = Name;
	return NewReaction;
}

void FChemicalReaction::ProcessReaction()
{
	UE_LOG(LogElementsChemistry, Display, TEXT("Reaction %s executing"), *ReactionId.ToString());

	
}
