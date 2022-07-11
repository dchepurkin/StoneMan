// Created by DChepurkin

#include "Animation/Notify/SMMakeDamageNotify.h"

#include "SMWeaponComponent.h"

void USMMakeDamageNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                      const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	const auto WeaponComponent = GetWeaponComponent(MeshComp);
	if(WeaponComponent) WeaponComponent->MakeDamage();
}

USMWeaponComponent* USMMakeDamageNotify::GetWeaponComponent(USkeletalMeshComponent* MeshComp)
{
	if(!MeshComp) return nullptr;

	const auto Actor = MeshComp->GetOwner();
	return Actor
		       ? Actor->FindComponentByClass<USMWeaponComponent>()
		       : nullptr;
}
