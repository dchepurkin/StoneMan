// Created by DChepurkin

#include "Environments/Doors/SMDoorBase.h"

ASMDoorBase::ASMDoorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	ArchMesh = CreateDefaultSubobject<UStaticMeshComponent>("ArchMesh");
	SetRootComponent(ArchMesh);

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>("DoorMesh");
	DoorMesh->SetupAttachment(ArchMesh);

	OpenDoorTimeline = CreateDefaultSubobject<UTimelineComponent>("OpenDoorTimeline");
}

void ASMDoorBase::BeginPlay()
{
	Super::BeginPlay();

	OnOpenDoorTimelineFloat.BindDynamic(this, &ThisClass::OnOpenCloseDoor);
	if(OpenDoorCurve) OpenDoorTimeline->AddInterpFloat(OpenDoorCurve, OnOpenDoorTimelineFloat);
}

void ASMDoorBase::OpenDoor() const
{
	OpenDoorTimeline->Play();
}

void ASMDoorBase::CloseDoor() const
{
	OpenDoorTimeline->Reverse();
}
