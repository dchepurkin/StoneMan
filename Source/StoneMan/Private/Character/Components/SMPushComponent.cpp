// Created by DChepurkin

#include "Character/Components/SMPushComponent.h"

#include "SMPushableActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogPushComponent, All, All);

USMPushComponent::USMPushComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.TickInterval = 0.1f;
}

void USMPushComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TryToPush();
}

void USMPushComponent::StartPush()
{
	SetComponentTickEnabled(true);
}

void USMPushComponent::StopPush()
{
	SetComponentTickEnabled(false);

	if(GetOwner()) GetOwner()->DetachFromActor({EDetachmentRule::KeepWorld, false});

	if(CurrentPushable)
	{
		CurrentPushable->StopMoving();
		CurrentPushable = nullptr;
	}

	OnStopPush.Broadcast();
}

void USMPushComponent::RestartPush()
{
	StopPush();
	StartPush();
}

void USMPushComponent::TryToPush()
{
	FHitResult HitResult;
	if(!LineTrace(HitResult)) return;

	CurrentPushable = Cast<ASMPushableActor>(HitResult.GetActor());
	if(!CanPush(CurrentPushable)) return;

	SetComponentTickEnabled(false);

	const auto PushRotation = CurrentPushable->GetClosestPushTransform(GetOwner()).GetRotation();
	GetOwner()->SetActorRotation(PushRotation);

	CurrentPushable->StartMoving(PushRotation.GetForwardVector());

	OnStartPush.Broadcast();
}

bool USMPushComponent::LineTrace(FHitResult& HitResult) const
{
	if(!GetWorld() || !GetOwner()) return false;

	const auto StartTrace = GetOwner()->GetActorLocation();
	const auto EndTrace = StartTrace + GetOwner()->GetActorForwardVector() * PushRange;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_WorldDynamic, CollisionQueryParams);

	return HitResult.bBlockingHit;
}

bool USMPushComponent::CanPush(ASMPushableActor* PushableActor) const
{
	if(IsOwnerFalling() || !PushableActor) return false;

	const auto PushTransform = PushableActor->GetClosestPushTransform(GetOwner());
	const auto PushForwardVector = PushTransform.GetRotation().GetForwardVector();

	if(!PushableActor->CanMove(PushForwardVector)) return false;

	const auto OwnerForwardVector = GetOwner()->GetActorForwardVector();
	
	const auto DotProduct = FVector::DotProduct(PushForwardVector, OwnerForwardVector);
	if(DotProduct < PushableAngle) return false;	

	FVector OwnerViewPointLocation;
	FRotator OwnerViewPointRotation;
	GetOwner()->GetActorEyesViewPoint(OwnerViewPointLocation, OwnerViewPointRotation);

	auto OwnerViewPointForwardVector = UKismetMathLibrary::GetForwardVector(OwnerViewPointRotation);
	OwnerViewPointForwardVector.Z = 0.f;
	OwnerViewPointForwardVector.Normalize();

	return OwnerForwardVector.Equals(OwnerViewPointForwardVector, 0.05f);
}

bool USMPushComponent::IsOwnerFalling() const
{
	if(!GetOwner()) return false;

	const auto MovementComponent = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();
	if(!MovementComponent) return false;

	return MovementComponent->IsFalling();
}
