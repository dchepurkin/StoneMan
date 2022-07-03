// Created by DChepurkin

#include "Animation/Notify/SMChangeElementNotify.h"

#include "SMPlayerCharacter.h"

void USMChangeElementNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                         const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	const auto Character = GetPlayerCharacter(MeshComp);
	if(Character) Character->ChangeColor();
}

void USMChangeElementNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	const auto Character = GetPlayerCharacter(MeshComp);
	if(Character) Character->SetState(ESMPlayerState::Idle);
}

ASMPlayerCharacter* USMChangeElementNotify::GetPlayerCharacter(USkeletalMeshComponent* MeshComp)
{
	return MeshComp ? MeshComp->GetOwner<ASMPlayerCharacter>() : nullptr;
}
