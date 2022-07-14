// Created by DChepurkin

#include "Character/Components/SMCharacterElementComponent.h"

#include "SMDamageTypeBase.h"

USMCharacterElementComponent::USMCharacterElementComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void USMCharacterElementComponent::BeginPlay()
{
	Super::BeginPlay();

	SetElement(Element);
}

float USMCharacterElementComponent::GetDamageFactor(const UDamageType* DamageType) const
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

void USMCharacterElementComponent::SetElement(const ESMElement NewElement)
{
	CurrentElement = NewElement;
	OnChangeElement.Broadcast();
}
