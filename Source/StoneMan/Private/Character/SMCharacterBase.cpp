// Created by DChepurkin

#include "SMCharacterBase.h"

#include "SMDestructComponent.h"
#include "Components/CapsuleComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

ASMCharacterBase::ASMCharacterBase(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	DestructComponent = CreateDefaultSubobject<USMDestructComponent>("DestructComponent");

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);
}

void ASMCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}
