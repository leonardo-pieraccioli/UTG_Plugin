// Fill out your copyright notice in the Description page of Project Settings.


#include "Definition/ChemicalMaterialDataAsset.h"

bool UChemicalMaterialDataAsset::operator==(UChemicalMaterialDataAsset& Rhs)
{
	return MaterialName == Rhs.MaterialName;
}
