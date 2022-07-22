// Created by DChepurkin

#include "Actors/SMWallLaser.h"

#include "SMElementComponent.h"
#include "SMLaserComponent.h"
#include "SMSwitchBase.h"
#include "Components/BoxComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogWallLaser, All, All);

ASMWallLaser::ASMWallLaser()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(StaticMesh);

	ElementComponent = CreateDefaultSubobject<USMElementComponent>("ElementComponent");

	LaserComponent = CreateDefaultSubobject<USMLaserComponent>("LaserComponent");
	LaserComponent->SetupAttachment(StaticMesh);

	LaserTrigger = CreateDefaultSubobject<UBoxComponent>("LaserTrigger");
	LaserTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	LaserTrigger->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	LaserTrigger->ComponentTags.Add("LaserTrigger");
	LaserTrigger->SetupAttachment(StaticMesh);
}

void ASMWallLaser::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UpdateColor();
}

void ASMWallLaser::BeginPlay()
{
	Super::BeginPlay();

	check(ElementComponent);
	check(LaserTrigger);
	check(LaserComponent);
	check(StaticMesh);

	LaserComponent->SetLaserDirection(GetActorForwardVector());

	LaserComponent->OnDetectLaserTrigger.AddUObject(this, &ThisClass::OnDetectLaserTrigger);
	LaserComponent->OnLoseLaserTrigger.AddUObject(this, &ThisClass::OnLoseLaserTrigger);

	UpdateLaserColor();
	
	BindSwitchesesDelegates();	
	if(!SwitchesesRules.IsEmpty()) OnChangeSwitch();
}

void ASMWallLaser::OnDetectLaserTrigger(AActor* Actor)
{
	if(!Actor || !Actor->Implements<USMLaserInterface>()) return;

	Cast<ISMLaserInterface>(Actor)->Activate(ElementComponent->GetElement());
}

void ASMWallLaser::OnLoseLaserTrigger(AActor* Actor)
{
	if(!Actor || !Actor->Implements<USMLaserInterface>()) return;

	Cast<ISMLaserInterface>(Actor)->Disactivate();
}

void ASMWallLaser::Activate(const ESMElement Element)
{
	if(!ElementComponent || !LaserComponent || LaserComponent->IsEnabled() || LaserComponent->IsUndisablable()) return;

	ElementComponent->SetElement(Element);
	LaserComponent->SetLaserEnabled(true);
	UpdateColor();
	UpdateLaserColor();
	SetNextLaserEnabled(true);
}

void ASMWallLaser::Disactivate()
{
	if(!ElementComponent || !LaserComponent || LaserComponent->IsUndisablable()) return;

	ElementComponent->SetElement(ESMElement::NoElement);
	UpdateColor();
	LaserComponent->SetLaserEnabled(false);
	SetNextLaserEnabled(false);
}

void ASMWallLaser::SetNextLaserEnabled(const bool Enabled) const
{
	if(!NextWallLaser) return;

	Enabled
		? NextWallLaser->Activate(ElementComponent->GetElement())
		: NextWallLaser->Disactivate();
}

void ASMWallLaser::UpdateColor() const
{
	if(!LaserComponent || !ElementComponent) return;

	if(!LaserComponent->IsEnabled()) ElementComponent->SetElement(ESMElement::NoElement);
	StaticMesh->SetVectorParameterValueOnMaterials(ElementComponent->GetMaterialParameterName(), FVector(ElementComponent->GetElementColor()));
}

void ASMWallLaser::UpdateLaserColor() const
{
	if(!LaserComponent || !ElementComponent) return;

	LaserComponent->SetLaserColor(ElementComponent->GetElementColor());
}

void ASMWallLaser::BindSwitchesesDelegates()
{
	if(SwitchesesRules.IsEmpty()) return;

	for(auto& [Switch, OpenOn] : SwitchesesRules)
	{
		if(!Switch) continue;

		Switch->OnChangeSwitchEnabled.AddUObject(this, &ThisClass::OnChangeSwitch);
	}
}

void ASMWallLaser::OnChangeSwitch()
{
	const auto Enabled = CheckForOpen();
	LaserComponent->SetLaserEnabled(Enabled);
	StaticMesh->SetScalarParameterValueOnMaterials(EmissiveParameterName, Enabled ? EmissiveON : EmissiveOFF);
}

bool ASMWallLaser::CheckForOpen()
{
	if(SwitchesesRules.IsEmpty()) return true;

	const auto CurrentSwitchRule = SwitchesesRules.FindByPredicate([](const FSwitches& SwitchRule)
	{
		if(!SwitchRule.Switch) return false;
		return SwitchRule.Switch->IsEnabled() ? SwitchRule.OpenOn != EOpenOn::OpenOnSwitchON : SwitchRule.OpenOn != EOpenOn::OpenOnSwitchOFF;
	});

	return CurrentSwitchRule ? false : true;
}
