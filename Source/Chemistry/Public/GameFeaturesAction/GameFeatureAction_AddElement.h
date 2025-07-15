// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "GameFeatureAction_AddElement.generated.h"

/**
 * 
 */
UCLASS()
class CHEMISTRY_API UGameFeatureAction_AddElement : public UGameFeatureAction
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly)
    TSet<TObjectPtr<class UElementDataAsset>> Elements;

public:
    virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;

private:
    void OnChemistrySubsystemCreated(class UChemistrySubsystem* ElementsSubsystem);

};
