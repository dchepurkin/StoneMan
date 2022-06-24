// Created by DChepurkin

#include "Character/Components/SMPushComponent.h"

#include "SMPushableActor.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogPushComponent, All, All);

USMPushComponent::USMPushComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void USMPushComponent::StartPush()
{
	if(!GetWorld()) return;

	GetWorld()->GetTimerManager().SetTimer(TryToPushTimerHandle, this, &ThisClass::TryToPush, 0.1f, true);
	TryToPush();
}

void USMPushComponent::StopPush()
{
	if(GetWorld()) GetWorld()->GetTimerManager().ClearTimer(TryToPushTimerHandle);

	if(GetOwner())
	{
		const FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, false);
		GetOwner()->DetachFromActor(DetachmentTransformRules);
	}

	if(CurrentPushable)
	{
		CurrentPushable->StopPush();
		CurrentPushable = nullptr;
	}

	OnStopPush.Broadcast();
}

void USMPushComponent::TryToPush()
{
	FHitResult HitResult;
	if(!LineTrace(HitResult)) return;

	CurrentPushable = Cast<ASMPushableActor>(HitResult.GetActor());
	if(!CanPush()) return;

	const auto PushRotation = CurrentPushable->GetClosestPushTransform(GetOwner()).GetRotation();
	if(GetOwner()) GetOwner()->SetActorRotation(PushRotation);
	CurrentPushable->StartPush(PushRotation.GetForwardVector());

	OnStartPush.Broadcast();
}

bool USMPushComponent::LineTrace(FHitResult& HitResult) const
{
	if(!GetWorld() || !GetOwner()) return false;

	const auto StartTrace = GetOwner()->GetActorLocation();
	const auto EndTrace = StartTrace + GetOwner()->GetActorForwardVector() * PushRange;

	constexpr ECollisionChannel CollisionChannel = ECC_WorldDynamic;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, CollisionChannel, CollisionQueryParams);

	return HitResult.bBlockingHit;
}

bool USMPushComponent::CanPush() const
{
	if(!GetOwner() || !CurrentPushable || !CurrentPushable->CanMove()) return false;

	const auto PushTransform = CurrentPushable->GetClosestPushTransform(GetOwner());
	const auto PushForwardVector = PushTransform.GetRotation().GetForwardVector();

	const auto DotProduct = FVector::DotProduct(PushForwardVector, GetOwner()->GetActorForwardVector());
	if(DotProduct < PushableAngle) return false;

	const auto OwnerForwardVector = GetOwner()->GetActorForwardVector();

	FVector OwnerViewPointLocation;
	FRotator OwnerViewPointRotation;
	GetOwner()->GetActorEyesViewPoint(OwnerViewPointLocation, OwnerViewPointRotation);

	auto OwnerViewPointForwardVector = UKismetMathLibrary::GetForwardVector(OwnerViewPointRotation);
	OwnerViewPointForwardVector.Z = 0.f;
	OwnerViewPointForwardVector.Normalize();

	return OwnerForwardVector.Equals(OwnerViewPointForwardVector, 0.05f);
}
