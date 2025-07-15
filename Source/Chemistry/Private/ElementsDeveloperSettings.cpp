// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementsDeveloperSettings.h"

FName UElementsDeveloperSettings::GetContainerName() const
{
	return TEXT("Project");
}

FName UElementsDeveloperSettings::GetCategoryName() const
{
	return TEXT("Game");
}

FName UElementsDeveloperSettings::GetSectionName() const
{
	return TEXT("Elements Plugin - Chemistry");
}

#if WITH_EDITOR
FText UElementsDeveloperSettings::GetSectionText() const
{
	return NSLOCTEXT("ElementsPlugin", "ElementsPluginSettings", "ElementsPlugin");
}

FText UElementsDeveloperSettings::GetSectionDescription() const
{
	return NSLOCTEXT("ElementsPluginChemistryDesc", "ElementsPluginChemistryDescSetting", "The default settings for the Elements Plugin Chemistry. Other features can be added with the Game Feature Actions.");
}
#endif

TArray<class UElementDataAsset*> UElementsDeveloperSettings::GetElements() const
{
	TArray<UElementDataAsset*> ElementsData;
	ElementsData.Reserve(Elements.Num());
	for (FSoftObjectPath ElementPath : Elements)
	{
		ElementsData.Add(Cast<class UElementDataAsset>(ElementPath.TryLoad()));
	}
	return MoveTemp(ElementsData);
}

TArray<class UChemicalMaterialDataAsset*> UElementsDeveloperSettings::GetChemicalMaterials() const
{
	TArray<class UChemicalMaterialDataAsset*> ChemicalMaterialData;
	ChemicalMaterialData.Reserve(Materials.Num());
	for (FSoftObjectPath MaterialPath : Materials)
	{
		ChemicalMaterialData.Add(Cast<class UChemicalMaterialDataAsset>(MaterialPath.TryLoad()));
	}
	return MoveTemp(ChemicalMaterialData);
}

TArray<class UReactionDataAsset*> UElementsDeveloperSettings::GetReactions() const
{
	TArray<class UReactionDataAsset*> ReactionsData;
	ReactionsData.Reserve(Reactions.Num());
	for (FSoftObjectPath ReactionPath : Reactions)
	{
		ReactionsData.Add(Cast<class UReactionDataAsset>(ReactionPath.TryLoad()));
	}
	return MoveTemp(ReactionsData);
}

TArray<class UCatalystDataAsset*> UElementsDeveloperSettings::GetCatalysts() const
{
	TArray<class UCatalystDataAsset*> CatalystsData;
	CatalystsData.Reserve(Catalysts.Num());
	for (FSoftObjectPath CatalystPath : Catalysts)
	{
		CatalystsData.Add(Cast<class UCatalystDataAsset>(CatalystPath.TryLoad()));
	}
	return MoveTemp(CatalystsData);
}


