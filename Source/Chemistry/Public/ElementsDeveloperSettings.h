// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Definition/ElementDataAsset.h"
#include "Definition/ReactionDataAsset.h"
#include "Definition/ChemicalMaterialDataAsset.h"
#include "Definition/CatalystDataAsset.h"
#include "ElementsDeveloperSettings.generated.h"

/**
 * Settings of Elements with all Data Assets
 */
UCLASS(config = Elements, defaultconfig, meta = (DisplayName = "Elements Plugin"))
class CHEMISTRY_API UElementsDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, config, meta = (AllowedClasses = "/Script/Chemistry.ChemicalMaterialDataAsset"))
	TSet<FSoftObjectPath> Materials;
	
	UPROPERTY(EditDefaultsOnly, config, meta = (AllowedClasses = "/Script/Chemistry.ElementDataAsset"))
	TSet<FSoftObjectPath> Elements;

	UPROPERTY(EditDefaultsOnly, config , meta = (AllowedClasses = "/Script/Chemistry.ReactionDataAsset"))
	TSet<FSoftObjectPath> Reactions;

	UPROPERTY(EditDefaultsOnly, config, meta = (AllowedClasses = "/Script/Chemistry.CatalystDataAsset"))
	TSet<FSoftObjectPath> Catalysts;

	// TODO: ExternalModifiers

public:
	/** Gets the settings container name for the settings, either Project or Editor */
	virtual FName GetContainerName() const override;
	/** Gets the category for the settings, some high level grouping like, Editor, Engine, Game...etc. */
	virtual FName GetCategoryName() const override;
	/** The unique name for your section of settings, uses the class's FName. */
	virtual FName GetSectionName() const override;

#if WITH_EDITOR
	/** Gets the section text, uses the classes DisplayName by default. */
	virtual FText GetSectionText() const override;
	/** Gets the description for the section, uses the classes ToolTip by default. */
	virtual FText GetSectionDescription() const override;
#endif

public:
	[[nodiscard]] TArray<class UElementDataAsset*> GetElements() const;
	[[nodiscard]] TArray<class UChemicalMaterialDataAsset*> GetChemicalMaterials() const;
	[[nodiscard]] TArray<class UReactionDataAsset*> GetReactions() const;
	[[nodiscard]] TArray<class UCatalystDataAsset*> GetCatalysts() const;
};
