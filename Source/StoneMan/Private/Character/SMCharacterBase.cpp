// Created by DChepurkin


#include "SMCharacterBase.h"

ASMCharacterBase::ASMCharacterBase(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASMCharacterBase::BeginPlay()
{
	Super::BeginPlay();	
}

