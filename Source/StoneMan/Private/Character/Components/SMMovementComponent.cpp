// Created by DChepurkin


#include "Character/Components/SMMovementComponent.h"

USMMovementComponent::USMMovementComponent()
{
	bUseControllerDesiredRotation = true;
	bOrientRotationToMovement = true;
	RotationRate = FRotator(0.f, 540.f, 0.f);
}
