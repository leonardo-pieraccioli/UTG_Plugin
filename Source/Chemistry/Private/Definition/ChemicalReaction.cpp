
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

bool FChemicalReaction::CheckReagents(TArray<FChemicalMaterial*> ProximityGroup)
{
	// Count occurrences in Reagents
    TMap<FName,int32> CountMap;
    CountMap.Reserve(Reagents.Num());
    for (const FReagentMaterialProperties& Reagent : Reagents)
        CountMap.FindOrAdd(Reagent.Material->GetType())++;

    // For each key in Subset, decrement count and fail if not enough
    for (FChemicalMaterial* Material : ProximityGroup)
    {
        int32* OccurrencyPtr = CountMap.Find(Material->GetType());
		if (!OccurrencyPtr)
			continue;
		(*OccurrencyPtr)--;
    }
	for (const auto& Occurrency : CountMap)
	{
		if (Occurrency.Value > 0)
			return false;
	}

	return true;
}

void FChemicalReaction::ProcessReaction()
{
	UE_LOG(LogElementsChemistry, Display, TEXT("Reaction %s executing"), *ReactionId.ToString());
}
