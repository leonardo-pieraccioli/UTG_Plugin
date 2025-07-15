// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ElementDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class CHEMISTRY_API UElementDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
private:
	float Energy;

public:
	UPROPERTY(EditDefaultsOnly, Category="ElementProperties")
	FName ElementName;

public:
	
	bool operator==(UElementDataAsset& Rhs);

	UFUNCTION()
	float ConsumeEnergy(float AmountToConsume);

	UFUNCTION()
	float ProvideEnergy(float AmountToProvide);

	//Get energy?
};
