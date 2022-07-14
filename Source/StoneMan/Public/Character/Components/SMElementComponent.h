// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "SMCoreTypes.h"
#include "Components/ActorComponent.h"
#include "SMElementComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STONEMAN_API USMElementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USMElementComponent();
	virtual ESMElement GetElement() const { return Element; }
	virtual UMaterialInstance* GetMaterial() const { return ElementMaterials[Element]; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SMElement)
	ESMElement Element = ESMElement::NoElement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMElement)
	TMap<ESMElement, UMaterialInstance*> ElementMaterials{
		{ESMElement::Ice, nullptr},
		{ESMElement::Fire, nullptr},
		{ESMElement::Earth, nullptr},
		{ESMElement::Air, nullptr},
		{ESMElement::NoElement, nullptr}
	};
};
