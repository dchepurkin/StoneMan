// Created by DChepurkin

#include "Components/SMLaserComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

USMLaserComponent::USMLaserComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USMLaserComponent::BeginPlay()
{
	Super::BeginPlay();

	Laser = UNiagaraFunctionLibrary::SpawnSystemAttached(LaserNiagaraSystem, this, NAME_None, GetRelativeLocation(), GetRelativeRotation(),
	                                                     EAttachLocation::SnapToTarget, false);

	check(Laser);
}

void USMLaserComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Laser->SetNiagaraVariableVec3(LaserEndParameterName, GetLaserEnd());
}

void USMLaserComponent::SetLaserColor(const FLinearColor& NewColor) const
{
	Laser->SetNiagaraVariableLinearColor(LaserColorParameterName, NewColor);
}

const FVector& USMLaserComponent::GetLaserEnd() const
{
	if(!GetOwner() || !GetWorld()) return FVector::ZeroVector;

	FHitResult HitResult;
	const auto Start = GetComponentLocation();
	const auto End = Start + GetForwardVector() * LaserMaxLength;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionQueryParams);

	return HitResult.bBlockingHit ? HitResult.ImpactPoint : End;
}
