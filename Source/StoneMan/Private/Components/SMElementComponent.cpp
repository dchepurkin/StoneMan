// Created by DChepurkin

#include "Components/SMElementComponent.h"

#include "SMDamageTypeBase.h"

USMElementComponent::USMElementComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void USMElementComponent::BeginPlay()
{
	Super::BeginPlay();
	SetElement(Element);
}

void USMElementComponent::SetElement(const ESMElement NewElement)
{
	Element = NewElement;
	OnChangeElement.Broadcast();
}

float USMElementComponent::GetDamageFactor(const UDamageType* DamageType) const
{
	auto DamageFactor = 1.f;
	if(DamageType->IsA(USMDamageTypeBase::StaticClass()))
	{
		if(const auto CurrentDamageType = DamageType->GetClass()->GetDefaultObject<USMDamageTypeBase>())
		{
			DamageFactor = CurrentDamageType->GetElementDamage(Element);
		}
	}
	return DamageFactor;
}


