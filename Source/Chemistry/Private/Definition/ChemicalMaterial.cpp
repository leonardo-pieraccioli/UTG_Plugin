


#include "Definition/ChemicalMaterial.h"
#include "ChemistryLog.h"

FName FChemicalMaterial::GetType()
{
	return Type;
}

FGuid FChemicalMaterial::GetID()
{
	return ID;
}

bool FChemicalMaterial::AttachElement(FChemicalElement NewElement)
{
	if (AttachedElements.Contains(NewElement.GetType()))
	{
		UE_LOG(LogElementsChemistry, Warning, TEXT("Element %s already attached to material %s."), *NewElement.GetType().ToString(), *Type.ToString());
		return false;
	}

	AttachedElements.Add(NewElement.GetType(), NewElement);
	return true;
}

bool FChemicalMaterial::RemoveElement(FName ElementName)
{
	if (!AttachedElements.Contains(ElementName))
	{
		UE_LOG(LogElementsChemistry, Warning, TEXT("Element %s not found in material %s."), *ElementName.ToString(), *Type.ToString());
		return false;
	}
	AttachedElements.Remove(ElementName);
	return true;
}

FChemicalElement* FChemicalMaterial::GetElement(FName ElementName)
{
	if (!AttachedElements.Contains(ElementName))
	{
		UE_LOG(LogElementsChemistry, Warning, TEXT("Element %s not found in material %s."), *ElementName.ToString(), *Type.ToString());
		return nullptr;
	}
	return AttachedElements.Find(ElementName);
}

float FChemicalMaterial::UpdateMass(float Amount)
{
	Quantity += Amount;
	return Quantity;
}

float FChemicalMaterial::UpdateEnergy(FName ElementName, float Amount, bool& ReturnStatus)
{
	if (!(AttachedElements.Contains(ElementName)))
	{
		ReturnStatus = false;
		UE_LOG(LogElementsChemistry, Error, TEXT("Element %s not found in material %s. Energy not updated."), *ElementName.ToString(), *Type.ToString());
		return 0.0f;
	}

	FChemicalElement* FoundElement = AttachedElements.Find(ElementName);
	FoundElement->Energy += Amount;

	ReturnStatus = true;
	return FoundElement->Energy;
}
