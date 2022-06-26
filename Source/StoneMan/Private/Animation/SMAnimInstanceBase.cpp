// Created by DChepurkin

#include "Animation/SMAnimInstanceBase.h"

#include "SMCharacterBase.h"

void USMAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ASMCharacterBase>(TryGetPawnOwner());
}

void USMAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(!Character) return;

	const auto Velocity = Character->GetVelocity();
	Speed = Velocity.Length();	
}


