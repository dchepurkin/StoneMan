// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "Character/Components/SMElementComponent.h"
#include "SMCharacterElementComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnChangeElementSignature)

UCLASS()
class STONEMAN_API USMCharacterElementComponent : public USMElementComponent
{
	GENERATED_BODY()

public:
	FOnChangeElementSignature OnChangeElement;

	USMCharacterElementComponent();
	virtual void BeginPlay() override;
	float GetDamageFactor(const UDamageType* DamageType) const;
	void SetElement(const ESMElement NewElement);
	virtual UMaterialInstance* GetMaterial() const override { return ElementMaterials[CurrentElement]; }
	virtual ESMElement GetElement() const override { return CurrentElement; };

private:
	ESMElement CurrentElement = ESMElement::Ice;
};
