// Created by DChepurkin

#include "Animation/Notify/SMWeaponVisibilityNotify.h"

#include "SMWeaponComponent.h"

void USMWeaponVisibilityNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                            const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if(!MeshComp) return;

	const auto Actor = MeshComp->GetOwner();
	if(!Actor) return;

	const auto WeaponComponent = Actor->FindComponentByClass<USMWeaponComponent>();
	if(!WeaponComponent) return;

	WeaponComponent->ShowWeapon(Visible);
}
