


#include "Definition/ChemicalElement.h"

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
