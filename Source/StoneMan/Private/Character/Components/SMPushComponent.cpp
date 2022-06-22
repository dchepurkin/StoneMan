// Created by DChepurkin

#include "Character/Components/SMPushComponent.h"

USMPushComponent::USMPushComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USMPushComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USMPushComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
