// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "SMCoreTypes.h"
#include "GameFramework/DamageType.h"
#include "SMDamageTypeBase.generated.h"

UCLASS()
class STONEMAN_API USMDamageTypeBase : public UDamageType
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	float GetElementDamage(const ESMElement Element) { return ElementDamage[Element]; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMDamageType)
	TMap<ESMElement, float> ElementDamage{
		{ESMElement::Ice, 1.f},
		{ESMElement::Fire, 1.f},
		{ESMElement::Earth, 1.f},
		{ESMElement::Air, 1.f},
	};
};
