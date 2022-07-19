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
	virtual void BeginPlay() override;
	virtual ESMElement GetElement() const { return Element; }
	virtual const FLinearColor& GetElementColor() const { return ElementColors[Element]; }
	virtual UMaterialInstance* GetElementMaterial() const { return ElementMaterials[Element]; }
	const FName& GetMaterialParameterName() const { return MaterialParameterName; }
	const int32& GetMaterialElementIndex() const { return MaterialElementIndex; }

	void SetElement(const ESMElement NewElement);
	float GetDamageFactor(const UDamageType* DamageType) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SMElement)
	ESMElement Element = ESMElement::NoElement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMElement)
	TMap<ESMElement, FLinearColor> ElementColors{
		{ESMElement::Ice, FLinearColor(0.012557f, 0.231279f, 1.f)},
		{ESMElement::Fire, FLinearColor(0.678431f, 0.035294f, 0.035294f)},
		{ESMElement::Earth, FLinearColor(0.141176f, 0.05098f, 0.019608f)},
		{ESMElement::Air, FLinearColor(0.717647f, 0.741176f, 0.854902f)},
		{ESMElement::NoElement, FLinearColor(0.05098f, 0.05098f, 0.05098f)}
	};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMElement)
	TMap<ESMElement, UMaterialInstance*> ElementMaterials{
		{ESMElement::Ice, nullptr},
		{ESMElement::Fire, nullptr},
		{ESMElement::Earth, nullptr},
		{ESMElement::Air, nullptr},
		{ESMElement::NoElement, nullptr}
	};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMElement)
	FName MaterialParameterName = "BaseColor";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMElement, meta=(ClampMin=0))
	int32 MaterialElementIndex = 0;
};
