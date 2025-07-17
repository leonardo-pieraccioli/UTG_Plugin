// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeaturesAction/GameFeatureAction_AddElement.h"
#include "ChemistrySubsystem.h"

void UGameFeatureAction_AddElement::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	Super::OnGameFeatureActivating(Context);
	UChemistrySubsystem::RegisterOnElementsSubsystemCreated(UChemistrySubsystem::FOnElementsSubsystemCreated::FDelegate::CreateUObject(this, &ThisClass::OnChemistrySubsystemCreated));
}

void UGameFeatureAction_AddElement::OnChemistrySubsystemCreated(class UChemistrySubsystem* ElementsSubsystem)
{
	for (const UElementDataAsset* Element : Elements)
	{
		// ElementsSubsystem->AddElement(Element);
	}
}