// Created by DChepurkin

#include "Actors/SMWallSwitch.h"

DEFINE_LOG_CATEGORY_STATIC(LogWallSwitch, All, All);

ASMWallSwitch::ASMWallSwitch()
{
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);

	ArmMesh = CreateDefaultSubobject<UStaticMeshComponent>("ArmMesh");
	ArmMesh->SetupAttachment(BaseMesh);

	SwitchTimeline = CreateDefaultSubobject<UTimelineComponent>("SwitchTimeline");
	CalculateRotation();
}

void ASMWallSwitch::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetSwitchRotation(IsEnabled ? EnabledRotation : DisabledRotation);
	SetLightColor(IsEnabled);
}

void ASMWallSwitch::BeginPlay()
{
	Super::BeginPlay();

	check(BaseMesh);
	check(ArmMesh);

	OnSwitchTimeline.BindDynamic(this, &ThisClass::ASMWallSwitch::OnSwitchTimelineCallback);
	OnSwitchTimelineFinished.BindDynamic(this, &ThisClass::OnSwitchTimelineFinishedCallback);
	SwitchTimeline->AddInterpFloat(SwitchCurve, OnSwitchTimeline);
	SwitchTimeline->SetTimelineFinishedFunc(OnSwitchTimelineFinished);

	SetSwitchEnabled(IsEnabled);
}

void ASMWallSwitch::Interact(AActor* WhoInteract)
{
	!bSwitched
		? SwitchTimeline->Play()
		: SwitchTimeline->Reverse();
}

void ASMWallSwitch::OnSwitchTimelineCallback(float Alpha)
{
	const auto CurrentRotation = FMath::Lerp(DisabledRotation, EnabledRotation, Alpha);
	SetSwitchRotation(CurrentRotation);
}

void ASMWallSwitch::OnSwitchTimelineFinishedCallback()
{
	SetSwitchEnabled(!bSwitched);
	SetLightColor(bSwitched);
}

void ASMWallSwitch::SetLightColor(const bool Enabled) const
{
	BaseMesh->SetVectorParameterValueOnMaterials(LightColorParameterName, Enabled ? FVector(ONColor) : FVector(OFFColor));
}

void ASMWallSwitch::SetSwitchRotation(const FRotator& NewRotation) const
{
	ArmMesh->SetRelativeRotation(NewRotation);
}

void ASMWallSwitch::CalculateRotation()
{
	DisabledRotation = ArmMesh->GetRelativeRotation();
	EnabledRotation = FRotator(DisabledRotation.Pitch + SwitchAngle, DisabledRotation.Yaw, DisabledRotation.Roll);
}
