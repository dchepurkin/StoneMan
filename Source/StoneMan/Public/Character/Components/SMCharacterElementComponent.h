// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "Components/SMElementComponent.h"
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
	virtual const FLinearColor& GetElementColor() const override { return ElementColors[CurrentElement]; }
	virtual ESMElement GetElement() const override { return CurrentElement; };

private:
	ESMElement CurrentElement = ESMElement::Ice;
};
