// Fill out your copyright notice in the Description page of Project Settings.


#include "Definition/CatalystDataAsset.h"

bool UCatalystDataAsset::operator=(UCatalystDataAsset& Rhs)
{
	return CatalystName == Rhs.CatalystName;
}

float FChemicalCatalyst::GetActivationThresholdModifier()
{
	return ActivationThresholdModifier;
}

FName FChemicalCatalyst::GetType()
{
	return Type;
}
