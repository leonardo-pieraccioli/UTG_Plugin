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