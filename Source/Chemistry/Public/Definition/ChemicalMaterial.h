

#pragma once

#include "CoreMinimal.h"
#include "Definition/ChemicalMaterialDataAsset.h"
#include "ChemicalElement.h"
#include "ChemicalMaterial.generated.h"

/**
 * Runtime data structure for chemical materials
 */
USTRUCT(BlueprintType)
struct FChemicalMaterial
{
	GENERATED_BODY()
public:
	// Default constructor
	FChemicalMaterial() : ID(FGuid::NewGuid()) {};
	// Constructor with Material Data Asset
	FChemicalMaterial(const UChemicalMaterialDataAsset* MaterialData) :
		Type(MaterialData->MaterialName),
		ID(FGuid::NewGuid()) {
	};
	// Copy and Move constructors
	FChemicalMaterial(const FChemicalMaterial& Other) :
		Type(Other.Type),
		ID(FGuid::NewGuid()),
		AttachedElements(Other.AttachedElements),
		Quantity(Other.Quantity),
		ProximityId(Other.ProximityId) {
	};
	FChemicalMaterial(FChemicalMaterial&& Other) noexcept :
		Type(MoveTemp(Other.Type)),
		ID(MoveTemp(Other.ID)),
		AttachedElements(MoveTemp(Other.AttachedElements)),
		Quantity(MoveTemp(Other.Quantity)),
		ProximityId(MoveTemp(Other.ProximityId)) {
	};
	// Copy and Move assignment operators
	FChemicalMaterial& operator=(const FChemicalMaterial& Rhs)
	{
		if (this != &Rhs)
		{
			Type = Rhs.Type;
			ID = Rhs.ID;
			AttachedElements = Rhs.AttachedElements;
			Quantity = Rhs.Quantity;
			ProximityId = Rhs.ProximityId;
		}
		return *this;
	};
	FChemicalMaterial& operator=(FChemicalMaterial&& Rhs) noexcept
	{
		if (this != &Rhs)
		{
			Type = MoveTemp(Rhs.Type);
			ID = MoveTemp(Rhs.ID);
			AttachedElements = MoveTemp(Rhs.AttachedElements);
			Quantity = MoveTemp(Rhs.Quantity);
			ProximityId = MoveTemp(Rhs.ProximityId);
		}
		return *this;
	};
	// Destructor
	~FChemicalMaterial() = default;

private:
	FName Type;
	FGuid ID;
	TMap<FName, FChemicalElement> AttachedElements;
	float Quantity;

public:
	UPROPERTY(BlueprintReadOnly, Category = "Proximity")
	FGuid ProximityId;

	// TODO: these functions are common to every entity, should implement a base class/interface for them
	FName GetType();
	FGuid GetID();
	FString GetIDString() { return *Type.ToString() + FString(" ") + ID.ToString(); };
	FString ToString() { return GetIDString(); };
	// -----------

	bool AttachElement(FChemicalElement NewElement);
	bool RemoveElement(FName ElementName);
	FChemicalElement* GetElement(FName ElementName);
	// ModifyElement();
	// TransformToMaterial();
	float UpdateMass(float Amount);
	float UpdateEnergy(FName ElementName, float Amount, bool& ReturnStatus);
};