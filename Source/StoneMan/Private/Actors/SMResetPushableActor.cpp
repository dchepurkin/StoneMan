// Created by DChepurkin

#include "Actors/SMResetPushableActor.h"

#include "SMPushableActor.h"
#include "SMSwitchBase.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ASMResetPushableActor::ASMResetPushableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	BaseMesh->CustomDepthStencilValue = 1;
	BaseMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	SetRootComponent(BaseMesh);

	ArmMesh = CreateDefaultSubobject<UStaticMeshComponent>("ArmMesh");
	ArmMesh->CustomDepthStencilValue = 1;
	ArmMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	ArmMesh->SetupAttachment(BaseMesh);

	Timeline = CreateDefaultSubobject<UTimelineComponent>("Timeline");
}

void ASMResetPushableActor::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloat OnTimelineFloat;
	OnTimelineFloat.BindDynamic(this, &ThisClass::OnTimeline);

	FOnTimelineEvent OnTimelineEnd;
	OnTimelineEnd.BindDynamic(this, &ThisClass::OnTimelineEnd);

	Timeline->AddInterpFloat(TimelineCurve, OnTimelineFloat);
	Timeline->SetTimelineFinishedFunc(OnTimelineEnd);

	StartRotation = ArmMesh->GetRelativeRotation();
	EndRotation = FRotator(StartRotation.Pitch + RotationAngle, StartRotation.Yaw, StartRotation.Roll);
}

void ASMResetPushableActor::Interact(AActor* WhoInteract)
{
	if(Timeline->IsPlaying() || !TimelineCurve) return;

	Timeline->PlayFromStart();
	PlaySound(StartSound);
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
		if(ActorToReset) ActorToReset->ResetCondition();
	}

	for(const auto SwitchToReset : SwitchesToReset)
	{
		if(SwitchToReset) SwitchToReset->ResetSwitch();
	}
}

void ASMResetPushableActor::PlaySound(USoundCue* Sound) const
{
	UGameplayStatics::PlaySoundAtLocation(this, Sound, GetActorLocation(), FRotator::ZeroRotator);
}

void ASMResetPushableActor::OnTimeline(const float Alpha)
{
	const auto CurrentRotaton = FMath::Lerp(StartRotation, EndRotation, Alpha);
	ArmMesh->SetRelativeRotation(CurrentRotaton);
}

void ASMResetPushableActor::OnTimelineEnd()
{
	if(!bGoBack)
	{
		bGoBack = true;
		ResetActors();
		PlaySound(MidleSound);
		Timeline->ReverseFromEnd();
	}
	else
	{
		bGoBack = false;
		PlaySound(EndSound);
	}
}
