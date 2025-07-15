// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeaturesAction/GameFeatureAction_AddReaction.h"


void UGameFeatureAction_AddReaction::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	Super::OnGameFeatureActivating(Context);
	// Register event on ElementsSubsystem creation
	// UChemicalElementsSubsystem::RegisterOnElementsSubsystemCreated(UChemicalElementsSubsystem::FOnElementsSubsystemCreated::FDelegate::CreateUObject(this, &ThisClass::OnElementsSubsystemCreated));
}