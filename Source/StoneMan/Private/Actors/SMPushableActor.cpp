// Created by DChepurkin

#include "Actors/SMPushableActor.h"

#include "K2Node_AddComponent.h"
#include "Components/ArrowComponent.h"

ASMPushableActor::ASMPushableActor()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(StaticMesh);
}

void ASMPushableActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASMPushableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASMPushableActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	AddArrowsComponents();
}

void ASMPushableActor::AddArrowsComponents()
{
	for(const auto PushTransform : PushTransforms)
	{
		AddComponentByClass(UArrowComponent::StaticClass(), false, PushTransform, false);
	}
}
