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

void UChemistrySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	const UElementsDeveloperSettings* DevSettings = GetDefault<UElementsDeveloperSettings>();
	
	// Initialize entities definitions
	for (const UElementDataAsset* Element : DevSettings->GetElements())
	{
		AddElement(Element);
		
	}
	// Materials
	// Reactions
	// Catalysts
}

void UChemistrySubsystem::AddChemicalMaterial(const UChemistryMaterialDataAsset* ChemistryMaterialData)
{

}

void UChemistrySubsystem::AddElement(const UElementDataAsset* ElementData)
{
	check(ElementData);

	if (Elements.Contains(ElementData))
	{
		UE_LOG(LogElementsChemistry, Error, TEXT("Element %s already declared!"), *ElementData->ElementName.ToString());
		return;
	}

	Elements.Add(ElementData);
	UE_LOG(LogElementsChemistry, Display, TEXT("Element %s added to subsystem!"), *ElementData->ElementName.ToString());
}
