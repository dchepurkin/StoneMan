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
	float GetElementDamage(const ESMCharacterElement Element) { return ElementDamage[Element]; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMDamageType)
	TMap<ESMCharacterElement, float> ElementDamage{
		{ESMCharacterElement::Ice, 1.f},
		{ESMCharacterElement::Fire, 1.f},
		{ESMCharacterElement::Earth, 1.f},
		{ESMCharacterElement::Air, 1.f},
	};
};
