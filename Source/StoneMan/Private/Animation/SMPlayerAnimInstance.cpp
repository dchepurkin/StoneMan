// Created by DChepurkin


#include "Animation/SMPlayerAnimInstance.h"

#include "SMPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void USMPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(!Character) return;
	
	const auto Velocity = Character->GetVelocity();
	const auto RightVector = Character->GetActorRightVector();
	LeanAxis = FVector::DotProduct(RightVector, Velocity);

	IsFalling = Character->GetCharacterMovement() ? Character->GetCharacterMovement()->IsFalling() : false;

	IsMakeSecondJump = TryToMakeSecondJump();

	PlayerState = GetPlayerCharacterState();
	
}

bool USMPlayerAnimInstance::TryToMakeSecondJump()
{
	if(!IsFalling) CanMakeSecondJump = true;

	if(!CanMakeSecondJump) return false;

	CanMakeSecondJump = !(Character->JumpCurrentCount == 2);
	return !CanMakeSecondJump;
}

ESMPlayerState USMPlayerAnimInstance::GetPlayerCharacterState() const
{
	const auto PlayerCharacter = Cast<ASMPlayerCharacter>(Character);
	if(!PlayerCharacter) return ESMPlayerState::Idle;

	return PlayerCharacter->GetState();
}