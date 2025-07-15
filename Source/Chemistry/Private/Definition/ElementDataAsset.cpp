// Fill out your copyright notice in the Description page of Project Settings.


#include "Definition/ElementDataAsset.h"

bool UElementDataAsset::operator==(UElementDataAsset& Rhs)
{
	return ElementName == Rhs.ElementName;
}

float UElementDataAsset::ConsumeEnergy(float AmountToConsume)
{
	float newEnergy = Energy - AmountToConsume;
	if (newEnergy <= 0) Energy = 0;
	else Energy = newEnergy;
	return Energy;
}

float UElementDataAsset::ProvideEnergy(float AmountToProvide)
{
	Energy += AmountToProvide;
	return AmountToProvide;
}