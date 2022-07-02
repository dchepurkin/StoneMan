// Created by DChepurkin

#include "Animation/Notify/SMEndAttackNotify.h"

#include "SMWeaponComponent.h"

void USMEndAttackNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if(!MeshComp) return;

	const auto Actor = MeshComp->GetOwner();
	if(!Actor) return;

	const auto WeaponComponent = Actor->FindComponentByClass<USMWeaponComponent>();
	if(!WeaponComponent) return;

	WeaponComponent->StopAttack();
}
