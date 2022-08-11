// Created by DChepurkin

#include "Actors/SMLaserSignaling.h"

#include "SMLaserComponent.h"

ASMLaserSignaling::ASMLaserSignaling()
{
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;

	bSwitched = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(StaticMesh);

	LaserComponent = CreateDefaultSubobject<USMLaserComponent>("LaserComponent");
	LaserComponent->SetReflect(false);
	LaserComponent->SetupAttachment(StaticMesh);
}

void ASMLaserSignaling::BeginPlay()
{
	Super::BeginPlay();

	check(LaserComponent);

	LaserComponent->SetLaserColor(LaserColor);
	LaserComponent->SetLaserDirection(GetActorForwardVector());
	LaserComponent->SetLaserEnabled(true);
	LaserComponent->OnDetectCharacter.AddUObject(this, &ThisClass::OnDetectCharacter);

	BindSwitchesesDelegates();	
	if(!SwitchesesRules.IsEmpty()) OnChangeSwitch();
}

void ASMLaserSignaling::OnDetectCharacter()
{
	SetSwitchEnabled(false);
}

void ASMLaserSignaling::ResetSwitch()
{
	SetSwitchEnabled(true);
}

void ASMLaserSignaling::BindSwitchesesDelegates()
{
	if(SwitchesesRules.IsEmpty()) return;

	for(auto& [Switch, OpenOn] : SwitchesesRules)
	{
		if(!Switch) continue;

		Switch->OnChangeSwitchEnabled.AddUObject(this, &ThisClass::OnChangeSwitch);
	}
}

void ASMLaserSignaling::OnChangeSwitch()
{
	const auto Enabled = !CheckForOpen();
	LaserComponent->SetLaserEnabled(Enabled);
}

bool ASMLaserSignaling::CheckForOpen()
{
	if(SwitchesesRules.IsEmpty()) return true;

	const auto CurrentSwitchRule = SwitchesesRules.FindByPredicate([](const FSwitches& SwitchRule)
	{
		if(!SwitchRule.Switch) return false;
		return SwitchRule.Switch->IsEnabled() ? SwitchRule.OpenOn != EOpenOn::OpenOnSwitchON : SwitchRule.OpenOn != EOpenOn::OpenOnSwitchOFF;
	});

	return CurrentSwitchRule ? false : true;
}
