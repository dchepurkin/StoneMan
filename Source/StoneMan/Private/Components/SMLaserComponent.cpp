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
	
	if(IsDamaged) SetDamageTimerEnabled(HitResult.GetActor());
	else CheckForPriviewLaser(HitResult) ? TryToDetectLaserTrigger(HitResult) : ClearLaserTrigger();

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

	if(!bEnabled) ClearLaserTrigger();
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
	if(!HitResult.GetActor()) return false;
	if(!HitResult.GetActor()->FindComponentByClass<USMLaserComponent>()) return true;

	return HitResult.GetActor()->FindComponentByClass<USMLaserComponent>()->CurrentLaserTrigger != GetOwner();
}

void USMLaserComponent::MakeDamage(AActor* DamagedActor) const
{
	if(DamagedActor) DamagedActor->TakeDamage(Damage, FDamageEvent(), nullptr, nullptr);
}

void USMLaserComponent::SetDamageTimerEnabled(AActor* DamagedActor)
{
	if(!GetWorld()) return;

	if(DamagedActor && DamagedActor != CurrenDamagedActor)
	{
		CurrenDamagedActor = DamagedActor;
		FTimerDelegate DamageTimerDelegate;
		DamageTimerDelegate.BindUObject(this, &ThisClass::MakeDamage, CurrenDamagedActor);

		GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, DamageTimerDelegate, DamageRate, true);
	}
	else if(!DamagedActor)
	{
		CurrenDamagedActor = nullptr;
		GetWorld()->GetTimerManager().ClearTimer(DamageTimerHandle);
	}
}

void USMLaserComponent::TryToDetectLaserTrigger(const FHitResult& HitResult)
{
	if(HitResult.GetComponent() && HitResult.GetComponent()->ComponentHasTag(LaserTriggerTag))
	{
		CurrentLaserTrigger = HitResult.GetActor();
		OnDetectLaserTrigger.Broadcast(CurrentLaserTrigger);
	}
	else ClearLaserTrigger();
}

void USMLaserComponent::ClearLaserTrigger()
{
	OnLoseLaserTrigger.Broadcast(CurrentLaserTrigger);
	CurrentLaserTrigger = nullptr;
}
