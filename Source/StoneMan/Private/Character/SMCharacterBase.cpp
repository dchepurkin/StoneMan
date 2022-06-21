// Created by DChepurkin

#include "SMCharacterBase.h"

#include "GeometryCollection/GeometryCollectionComponent.h"

ASMCharacterBase::ASMCharacterBase(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	CreateGeometryCollectionComponent(Head, "Head");
	CreateGeometryCollectionComponent(Tors, "Tors");
	CreateGeometryCollectionComponent(LeftShoulder, "LeftShoulder");
	CreateGeometryCollectionComponent(RightShoulder, "RightShoulder");
	CreateGeometryCollectionComponent(LeftHand, "LeftHand");
	CreateGeometryCollectionComponent(RightHand, "RightHand");
	CreateGeometryCollectionComponent(Belt, "Belt");
	CreateGeometryCollectionComponent(LeftLeg, "LeftLeg");
	CreateGeometryCollectionComponent(RightLeg, "RightLeg");
}

void ASMCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	Head->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, HeadSocketName);
	Tors->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TorsSocketName);
	LeftShoulder->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, LeftShoulderSocketName);
	RightShoulder->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, RightShoulderSocketName);
	LeftHand->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, LeftHandSocketName);
	RightHand->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, RightHandSocketName);
	Belt->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, BeltSocketName);
	LeftLeg->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, LeftLegSocketName);
	RightLeg->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, RightLegSocketName);
}

void ASMCharacterBase::CreateGeometryCollectionComponent(UGeometryCollectionComponent*& GeometryCollection, const FName& Name)
{
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(Name);
	GeometryCollection->SetSimulatePhysics(false);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Ignore);
}
