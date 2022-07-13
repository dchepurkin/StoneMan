// Created by DChepurkin


#include "Actors/SMSwitchBase.h"
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
	bSwitched = Enabled;
	OnChangeSwitchEnabled.Broadcast(this, bSwitched);	
}

