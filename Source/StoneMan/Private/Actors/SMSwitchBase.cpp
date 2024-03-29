// Created by DChepurkin

#include "Actors/SMSwitchBase.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ASMSwitchBase::ASMSwitchBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASMSwitchBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASMSwitchBase::SetSwitchEnabled(const bool Enabled)
{
	if(bSwitched == Enabled) return;
	
	bSwitched = Enabled;	
	OnChangeSwitchEnabled.Broadcast();

	UGameplayStatics::PlaySoundAtLocation(this, bSwitched ? SwitchONSound : SwitchOFFSound, GetActorLocation());
}
