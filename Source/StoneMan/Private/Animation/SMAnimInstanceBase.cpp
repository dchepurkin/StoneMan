// Created by DChepurkin

#include "Animation/SMAnimInstanceBase.h"

#include "SMPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	const auto RightVector = Character->GetActorRightVector();
	LeanAxis = FVector::DotProduct(RightVector, Velocity);

	IsFalling = Character->GetCharacterMovement() ? Character->GetCharacterMovement()->IsFalling() : false;

	IsMakeSecondJump = TryToMakeSecondJump();

	PlayerState = GetPlayerCharacterState();
}

bool USMAnimInstanceBase::TryToMakeSecondJump()
{
	if(!IsFalling) CanMakeSecondJump = true;

	if(!CanMakeSecondJump) return false;

	CanMakeSecondJump = !(Character->JumpCurrentCount == 2);
	return !CanMakeSecondJump;
}

ESMPlayerState USMAnimInstanceBase::GetPlayerCharacterState() const
{
	const auto PlayerCharacter = Cast<ASMPlayerCharacter>(Character);
	if(!PlayerCharacter) return ESMPlayerState::Idle;

	return PlayerCharacter->GetState();
}
