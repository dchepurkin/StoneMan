// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "SMCoreTypes.h"
#include "Components/ActorComponent.h"
#include "SMElementComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnChangeElementSignature)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STONEMAN_API USMElementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnChangeElementSignature OnChangeElement;

	USMElementComponent();
	float GetDamageFactor(const UDamageType* DamageType) const;
	void SetElement(const ESMCharacterElement NewElement);
	ESMCharacterElement GetElement() const { return CurrentElement; }
	UMaterialInstance* GetMaterial() const { return ElementMaterials[CurrentElement]; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SMElement)
	ESMCharacterElement Element;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SMElement)
	TMap<ESMCharacterElement, UAnimMontage*> ChangeElementAnimations{
		{ESMCharacterElement::Ice, nullptr},
		{ESMCharacterElement::Fire, nullptr},
		{ESMCharacterElement::Earth, nullptr},
		{ESMCharacterElement::Air, nullptr}
	};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SMElement)
	TMap<ESMCharacterElement, UMaterialInstance*> ElementMaterials{
		{ESMCharacterElement::Ice, nullptr},
		{ESMCharacterElement::Fire, nullptr},
		{ESMCharacterElement::Earth, nullptr},
		{ESMCharacterElement::Air, nullptr}
	};

private:
	ESMCharacterElement CurrentElement = ESMCharacterElement::Ice;
};
