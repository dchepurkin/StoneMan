// Created by DChepurkin

#include "Actors/SMPushableActor.h"

#include "Components/ArrowComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogPushableActor, All, All);

ASMPushableActor::ASMPushableActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(StaticMesh);
}

void ASMPushableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASMPushableActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#ifdef WITH_EDITOR
	AddArrowsComponents();
#endif
}

void ASMPushableActor::AddArrowsComponents()
{
	for(const auto PushTransform : PushTransforms)
	{
		AddComponentByClass(UArrowComponent::StaticClass(), false, PushTransform, false);
	}
}

const FTransform& ASMPushableActor::GetClosestPushTransform(const AActor* PushingActor)
{
	if(!PushingActor || PushTransforms.IsEmpty()) return FTransform::Identity;

	int32 ClosestIndex = 0;

	auto PushTransformWorldLocation = GetActorTransform().TransformPosition(PushTransforms[ClosestIndex].GetLocation());
	auto ClosestDistance = FVector::DistXY(PushingActor->GetActorLocation(), PushTransformWorldLocation);

	for(auto i = 1; i < PushTransforms.Num(); ++i)
	{
		PushTransformWorldLocation = GetActorTransform().TransformPosition(PushTransforms[i].GetLocation());
		const auto CurrentDistance = FVector::DistXY(PushingActor->GetActorLocation(), PushTransformWorldLocation);
		
		if(CurrentDistance < ClosestDistance)
		{
			ClosestDistance = CurrentDistance;
			ClosestIndex = i;
		}
	}
	
	return PushTransforms[ClosestIndex];
}
