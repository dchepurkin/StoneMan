// Created by DChepurkin

#include "Character/Components/SMCameraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogCameraComponent, All, All);

USMCameraComponent::USMCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.1f;
}

void USMCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckCameraCollision();
}

void USMCameraComponent::CheckCameraCollision()
{
	if(!GetWorld()) return;

	const auto StartPoint = GetComponentLocation();
	const auto Direction = GetForwardVector() * FadeDistance;
	const auto EndPoint = StartPoint + Direction;

	FHitResult HitResult;

	FCollisionObjectQueryParams CollisionObjectQueryParams;
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	GetWorld()->LineTraceSingleByObjectType(HitResult, StartPoint, EndPoint, CollisionObjectQueryParams);

	if(!HitResult.bBlockingHit && IsOverlaped)
	{
		IsOverlaped = false;
		OnCameraEndOverlap.Broadcast();
	}
	else if(HitResult.bBlockingHit && !IsOverlaped)
	{
		IsOverlaped = true;
		OnCameraBeginOverlap.Broadcast(HitResult.GetActor());
	}
}
