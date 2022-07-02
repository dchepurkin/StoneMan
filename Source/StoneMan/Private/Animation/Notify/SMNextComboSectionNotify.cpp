// Created by DChepurkin

#include "Animation/Notify/SMNextComboSectionNotify.h"

#include "SMWeaponComponent.h"

void USMNextComboSectionNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                            const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if(const auto WeaponComponent = GetWeaponComponent(MeshComp))
		WeaponComponent->SetNextComboSectionEnabled(true, NextComboSectionName);
}

void USMNextComboSectionNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if(const auto WeaponComponent = GetWeaponComponent(MeshComp))
		WeaponComponent->SetNextComboSectionEnabled(false);
}

USMWeaponComponent* USMNextComboSectionNotify::GetWeaponComponent(const USkeletalMeshComponent* MeshComp) const
{
	if(!MeshComp) return nullptr;

	const auto Actor = MeshComp->GetOwner();
	if(!Actor) return nullptr;

	return Actor->FindComponentByClass<USMWeaponComponent>();
}
