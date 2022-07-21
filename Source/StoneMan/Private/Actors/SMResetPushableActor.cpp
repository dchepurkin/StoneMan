// Created by DChepurkin

#include "Actors/SMResetPushableActor.h"

#include "SMPushableActor.h"
#include "Components/TimelineComponent.h"

ASMResetPushableActor::ASMResetPushableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	BaseMesh->CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	BaseMesh->CustomDepthStencilValue = 1;
	SetRootComponent(BaseMesh);

	ArmMesh = CreateDefaultSubobject<UStaticMeshComponent>("ArmMesh");
	ArmMesh->CustomDepthStencilValue = 1;
	ArmMesh->SetupAttachment(BaseMesh);

	Timeline = CreateDefaultSubobject<UTimelineComponent>("Timeline");
}

void ASMResetPushableActor::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloat OnTimelineFloat;
	OnTimelineFloat.BindDynamic(this, &ThisClass::OnTimeline);

	FOnTimelineEvent OnTimelineEnd;
	OnTimelineEnd.BindDynamic(this, &ThisClass::ResetActors);

	Timeline->AddInterpFloat(TimelineCurve, OnTimelineFloat);
	Timeline->SetTimelineFinishedFunc(OnTimelineEnd);

	StartRotation = ArmMesh->GetRelativeRotation();
	EndRotation = FRotator(StartRotation.Pitch + RotationAngle, StartRotation.Yaw, StartRotation.Roll);
}

void ASMResetPushableActor::Interact(AActor* WhoInteract)
{
	if(!Timeline->IsPlaying()) Timeline->PlayFromStart();
}

void ASMResetPushableActor::SetOutlineVisible(const bool Visible)
{
	BaseMesh->SetRenderCustomDepth(Visible);
	ArmMesh->SetRenderCustomDepth(Visible);
}

void ASMResetPushableActor::ResetActors()
{
	for(const auto ActorToReset : ActorsToReset)
	{
		if(ActorToReset) ActorToReset->ResetLocation();
	}
}

void ASMResetPushableActor::OnTimeline(const float Alpha)
{
	const auto CurrentRotaton = FMath::Lerp(StartRotation, EndRotation, Alpha);
	ArmMesh->SetRelativeRotation(CurrentRotaton);
}
