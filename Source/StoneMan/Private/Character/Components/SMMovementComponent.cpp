// Created by DChepurkin

#include "Character/Components/SMMovementComponent.h"

USMMovementComponent::USMMovementComponent()
{
	bUseControllerDesiredRotation = true;
	bOrientRotationToMovement = true;
	RotationRate = FRotator(0.f, 400.f, 0.f);
	JumpZVelocity = 300.f;
	MaxAcceleration = 1500.f;
}
