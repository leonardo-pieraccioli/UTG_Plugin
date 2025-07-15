// Fill out your copyright notice in the Description page of Project Settings.


#include "Definition/ElementDataAsset.h"

bool UElementDataAsset::operator==(UElementDataAsset& Rhs)
{
	return ElementName == Rhs.ElementName;
}
