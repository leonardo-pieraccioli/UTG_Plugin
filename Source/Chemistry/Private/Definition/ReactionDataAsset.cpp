// Fill out your copyright notice in the Description page of Project Settings.


#include "Definition/ReactionDataAsset.h"
#include "ChemistrySubsystem.h"
#include "ChemistryLog.h"

FChemicalReaction::FChemicalReaction(const UReactionDataAsset* ReactionData) : Priority(ReactionData->Priority), ActivationThreshold(ReactionData->ActivationThreshold)
{
	Reagents.Materials.Reserve(ReactionData->Reagents.Materials.Num());
	Reagents.Catalysts.Reserve(ReactionData->Reagents.Catalysts.Num());

	Products.Materials.Reserve(ReactionData->Products.Materials.Num());
	Products.Catalysts.Reserve(ReactionData->Products.Catalysts.Num());
	Products.Elements.Reserve(ReactionData->Products.Elements.Num());
}

TArray<FName> FChemicalReaction::GetReagentMaterials()
{
	TArray<FName> ReagentMaterialsNames;
	ReagentMaterialsNames.Reserve(Reagents.Materials.Num());
	for (auto ReagentMaterials = Reagents.Materials; MaterialTuple Material : ReagentMaterials)
	{
		ReagentMaterialsNames.Add(Material.Material->GetType());
	} 
	return ReagentMaterialsNames;
}

TArray<FName> FChemicalReaction::GetReagentCatalysts()
{
	TArray<FName> ReagentCatalystsNames;
	ReagentCatalystsNames.Reserve(Reagents.Catalysts.Num());
	for (auto ReagentCatalysts = Reagents.Catalysts; CatalystTuple Catalyst : ReagentCatalysts)
	{
		ReagentCatalystsNames.Add(Catalyst.Catalyst->GetType());
	}
	return ReagentCatalystsNames;
}

TArray<FName> FChemicalReaction::GetProductMaterials()
{
	TArray<FName> ProductMaterialsNames;
	ProductMaterialsNames.Reserve(Products.Materials.Num());
	for (auto ProductMaterials = Products.Materials; MaterialTuple Material : ProductMaterials)
	{
		ProductMaterialsNames.Add(Material.Material->GetType());
	}
	return ProductMaterialsNames;
}

TArray<FName> FChemicalReaction::GetProductCatalysts()
{
	TArray<FName> ProductCatalystsNames;
	ProductCatalystsNames.Reserve(Products.Catalysts.Num());
	for (auto ProductCatalysts = Products.Catalysts; CatalystTuple Catalyst : ProductCatalysts)
	{
		ProductCatalystsNames.Add(Catalyst.Catalyst->GetType());
	}
	return ProductCatalystsNames;
}

TArray<FName> FChemicalReaction::GetProductElements()
{
	TArray<FName> ProductElementsNames;
	ProductElementsNames.Reserve(Products.Elements.Num());
	for (auto ProductElements = Products.Elements; ElementTuple Element : ProductElements)
	{
		ProductElementsNames.Add(Element.Element->GetType());
	}
	return ProductElementsNames;
}