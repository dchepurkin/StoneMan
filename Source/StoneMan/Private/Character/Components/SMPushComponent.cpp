// Created by DChepurkin

#include "Character/Components/SMPushComponent.h"

#include "SMPushableActor.h"

DEFINE_LOG_CATEGORY_STATIC(LogPushComponent, All, All);

USMPushComponent::USMPushComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void USMPushComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USMPushComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USMPushComponent::StartPush()
{
	if(!GetWorld()) return;

	GetWorld()->GetTimerManager().SetTimer(TryToPushTimerHandle, this, &ThisClass::TryToPush, 0.1f, true);
}

void USMPushComponent::StopPush()
{
	CurrentPushable = nullptr;
	if(!GetWorld()) return;

	GetWorld()->GetTimerManager().ClearTimer(TryToPushTimerHandle);
}

void USMPushComponent::TryToPush()
{
	if(!GetOwner()) return;

	FHitResult HitResult;
	if(!LineTrace(HitResult)) return;

	CurrentPushable = Cast<ASMPushableActor>(HitResult.GetActor());
	if(!CurrentPushable) return;

	const auto PushTransform = CurrentPushable->GetClosestPushTransform(GetOwner());
	const auto PushForwardVector = PushTransform.GetRotation().GetForwardVector();

	const auto DotProduct = FVector::DotProduct(PushForwardVector, GetOwner()->GetActorForwardVector());
	if(DotProduct >= PushableAngle) UE_LOG(LogPushComponent, Display, TEXT("START PUSH"));
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
