// Created by DChepurkin

#include "Character/Components/SMCameraComponent.h"

#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(LogCameraComponent, All, All);

USMCameraComponent::USMCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.1f;
}

void USMCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SetOwnerMeshVisibilityEnabled(!IsOnFadeDistance());
}

bool USMCameraComponent::IsOnFadeDistance() const
{
	if(!GetWorld() || !GetOwner()) return false;

	const auto StartPoint = GetComponentLocation();
	const auto Direction = GetForwardVector() * FadeDistance;
	const auto EndPoint = StartPoint + Direction;
	FHitResult HitResult;

	FCollisionObjectQueryParams CollisionObjectQueryParams;
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	GetWorld()->LineTraceSingleByObjectType(HitResult, StartPoint, EndPoint, CollisionObjectQueryParams);
	if(!HitResult.bBlockingHit) return false;

	return HitResult.GetActor() == GetOwner();
}

void USMCameraComponent::SetOwnerMeshVisibilityEnabled(const bool Enabled) const
{
	const auto Character = GetOwner<ACharacter>();
	if(!Character || !Character->GetMesh() || Character->GetMesh()->GetVisibleFlag() == Enabled) return;

	Character->GetMesh()->SetVisibility(Enabled, true);
}
