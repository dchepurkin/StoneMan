// Created by DChepurkin

#include "Components/SMLaserComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogLaserComponent, All, All);

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

	Undisabable = bEnabled;

	CurrentLaserDirection = LaserDirectionMap[LaserDirestion];
	SetLaserEnabled(bEnabled);
}

void USMLaserComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector LaserEnd;
	FHitResult HitResult;
	MakeLaser(LaserEnd, HitResult);
	CheckForPriviewLaser(HitResult) ? TryToDetectLaserTrigger(HitResult) : ClearNextLaser();

	Laser->SetNiagaraVariableVec3(LaserEndParameterName, LaserEnd);
}

void USMLaserComponent::SetLaserColor(const FLinearColor& NewColor) const
{
	Laser->SetNiagaraVariableLinearColor(LaserColorParameterName, NewColor);
}

void USMLaserComponent::RotateLaser()
{
	CurrentLaserDirection = CurrentLaserDirection.RotateAngleAxis(90, FVector(0.f, 0.f, 1.f));
}

void USMLaserComponent::SetLaserEnabled(const bool Enabled)
{
	bEnabled = Enabled;
	SetComponentTickEnabled(bEnabled);
	Laser->SetVisibility(bEnabled);

	if(!bEnabled) ClearNextLaser();
}

void USMLaserComponent::MakeLaser(FVector& LaserEnd, FHitResult& HitResult) const
{
	const auto LaserStart = GetComponentLocation();
	LaserEnd = LaserStart + CurrentLaserDirection * LaserMaxLength;

	if(!GetOwner() || !GetWorld()) return;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HitResult, LaserStart, LaserEnd, ECC_Visibility, CollisionQueryParams);
	if(HitResult.bBlockingHit) LaserEnd = HitResult.ImpactPoint;
}

bool USMLaserComponent::CheckForPriviewLaser(const FHitResult& HitResult) const
{
	return HitResult.GetActor()
		&& HitResult.GetActor()->FindComponentByClass<USMLaserComponent>()
		&& HitResult.GetActor()->FindComponentByClass<USMLaserComponent>()->CurrentNextLaser != GetOwner();
}

void USMLaserComponent::TryToDetectLaserTrigger(const FHitResult& HitResult)
{
	if(HitResult.GetComponent() && HitResult.GetComponent()->ComponentHasTag(LaserTriggerTag))
	{
		CurrentNextLaser = HitResult.GetActor();
		OnDetectLaserTrigger.Broadcast(CurrentNextLaser);
	}
	else ClearNextLaser();
}

void USMLaserComponent::ClearNextLaser()
{
	OnLoseLaserTrigger.Broadcast(CurrentNextLaser);
	CurrentNextLaser = nullptr;
}
