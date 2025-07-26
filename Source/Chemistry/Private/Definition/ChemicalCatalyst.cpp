


#include "Definition/ChemicalCatalyst.h"

float FChemicalCatalyst::GetActivationThresholdModifier()
{
	return ActivationThresholdModifier;
}

FName FChemicalCatalyst::GetType()
{
	return Type;
}

FGuid FChemicalCatalyst::GetID()
{
	return ID;
}