// Created by DChepurkin

#include "Character/Components/SMElementComponent.h"

#include "SMDamageTypeBase.h"

USMElementComponent::USMElementComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USMElementComponent::BeginPlay()
{
	Super::BeginPlay();
}

float USMElementComponent::GetDamageFactor(const UDamageType* DamageType) const
{
	auto DamageFactor = 1.f;
	if(DamageType->IsA(USMDamageTypeBase::StaticClass()))
	{
		if(const auto CurrentDamageType = DamageType->GetClass()->GetDefaultObject<USMDamageTypeBase>())
		{
			DamageFactor = CurrentDamageType->GetElementDamage(CurrentElement);
		}
	}
	return DamageFactor;
}

void USMElementComponent::SetElement(const ESMCharacterElement NewElement)
{
	CurrentElement = NewElement;
}
