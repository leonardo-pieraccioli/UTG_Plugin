// Fill out your copyright notice in the Description page of Project Settings.


#include "Definition/ElementDataAsset.h"

bool UElementDataAsset::operator==(UElementDataAsset& Rhs)
{
	return ElementName == Rhs.ElementName;
}

FName FChemicalElement::GetType()
{
	return Type;
}

float FChemicalElement::ConsumeEnergy(float Amount)
{
	Energy -= Amount;
	return Energy;
}

float FChemicalElement::ProvideEnergy(float Amount)
{
	Energy += Amount;
	return Energy;
}
