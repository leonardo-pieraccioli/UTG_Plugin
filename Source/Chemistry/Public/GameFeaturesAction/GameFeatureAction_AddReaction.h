// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "GameFeatureAction_AddReaction.generated.h"

/**
 * 
 */
UCLASS()
class CHEMISTRY_API UGameFeatureAction_AddReaction : public UGameFeatureAction
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<int> Test;

public:
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;

};
