

#pragma once

#include "CoreMinimal.h"
#include "ReactionDataAsset.h"
#include "ChemicalElement.h"
#include "ChemicalMaterial.h"
#include "ChemicalCatalyst.h"
#include "ChemicalReaction.generated.h"

/**
 * Runtime data structure for Chemical Reactions
 */
USTRUCT(BlueprintType)
struct FChemicalReaction
{
	GENERATED_BODY()

private:
	// TODO: Maybe coefficients are not intuitive, we could use a more descriptive name like "Amount" or "Quantity"
	struct FElementTuple
	{
		float Coefficient;
		FChemicalElement* Element;
	};

	struct FCatalystTuple
	{
		float Coefficient;
		FChemicalCatalyst* Catalyst;
	};

	struct FMaterialTuple
	{
		float Coefficient;
		FChemicalMaterial* Material;
	};

	struct FProductEntities
	{
		TArray<FMaterialTuple> Materials;
		TArray<FCatalystTuple> Catalysts;
		TArray<FElementTuple> Elements;
	};

	struct FReagentMaterialProperties
	{
		float Coefficient;
		FChemicalMaterial* Material;
		FName ActivationElement;
		float ActivationThreshold;
	};

public:
	// Default constructor generates a new unique ID for the reaction
	FChemicalReaction() : ReactionId(FGuid::NewGuid()) {};
	// Constructor that initializes the reaction from a ReactionDataAsset
	FChemicalReaction(const UReactionDataAsset* ReactionData);
	// Copy and Move constructors
	FChemicalReaction(const FChemicalReaction& Other) :
		ReactionId(FGuid::NewGuid()),
		Name(Other.Name),
		Products(Other.Products),
		Reagents(Other.Reagents),
		Priority(Other.Priority)
	{
	};
	FChemicalReaction(FChemicalReaction&& Other) noexcept :
		ReactionId(Other.ReactionId),
		Name(MoveTemp(Other.Name)),
		Products(MoveTemp(Other.Products)),
		Reagents(MoveTemp(Other.Reagents)),
		Priority(Other.Priority)
	{
		Other.ReactionId.Invalidate();
	};
	// Copy and Move assignment operators
	FChemicalReaction& operator=(const FChemicalReaction& Rhs)
	{
		if (this != &Rhs)
		{
			ReactionId = Rhs.ReactionId;
			Name = Rhs.Name;
			Products = Rhs.Products;
			Reagents = Rhs.Reagents;
			Priority = Rhs.Priority;
		}
		return *this;
	};
	FChemicalReaction& operator=(FChemicalReaction&& Rhs) noexcept
	{
		if (this != &Rhs)
		{
			ReactionId = Rhs.ReactionId;
			Name = MoveTemp(Rhs.Name);
			Products = MoveTemp(Rhs.Products);
			Reagents = MoveTemp(Rhs.Reagents);
			Priority = Rhs.Priority;

			Rhs.ReactionId.Invalidate();
		}
		return *this;
	};
	// Destructor
	~FChemicalReaction() = default;

private:
	FGuid ReactionId;

public:
	FName Name;
	FProductEntities Products;
	TArray<FReagentMaterialProperties> Reagents;
	int32 Priority;

	TArray<FName> GetReagentMaterials();
	TArray<TTuple<FName, float>> GetReagentThresholds();
	TArray<FName> GetProductMaterials();
	TArray<FName> GetProductCatalysts();
	TArray<FName> GetProductElements();


	void ProcessReaction();
};