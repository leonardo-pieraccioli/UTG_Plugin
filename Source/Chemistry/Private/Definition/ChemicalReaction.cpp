
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

TArray<TTuple<FName, float>> FChemicalReaction::GetReagentThresholds()
{
	TArray<TTuple<FName, float>> ReagentThresholds;
	for (FReagentMaterialProperties Reagent : Reagents)
	{
		ReagentThresholds.Emplace(Reagent.Material->GetType(), Reagent.ActivationThreshold);
	}
	return ReagentThresholds;
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

void FChemicalReaction::ProcessReaction()
{
	UE_LOG(LogElementsChemistry, Display, TEXT("Reaction %s executing"), *ReactionId.ToString());
}
