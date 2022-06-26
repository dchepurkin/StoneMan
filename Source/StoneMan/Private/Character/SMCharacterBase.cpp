// Created by DChepurkin

#include "SMCharacterBase.h"

#include "SMDestructComponent.h"
#include "SMHealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ASMCharacterBase::ASMCharacterBase(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	DestructComponent = CreateDefaultSubobject<USMDestructComponent>("DestructComponent");
	HealthComponent = CreateDefaultSubobject<USMHealthComponent>("HealthComponent");

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);
}

void ASMCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnDeath.AddUObject(this, &ThisClass::OnDeath);
}

void ASMCharacterBase::OnDeath()
{
	GetCharacterMovement()->DisableMovement();
	DestructComponent->Destruct();
}
