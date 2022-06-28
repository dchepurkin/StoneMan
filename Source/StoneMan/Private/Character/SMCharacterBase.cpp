// Created by DChepurkin

#include "SMCharacterBase.h"

#include "SMDamageTypeBase.h"
#include "SMDestructComponent.h"
#include "SMElementComponent.h"
#include "SMHealthComponent.h"
#include "SMWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogCharacterBase, All, All);

ASMCharacterBase::ASMCharacterBase(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	DestructComponent = CreateDefaultSubobject<USMDestructComponent>("DestructComponent");
	HealthComponent = CreateDefaultSubobject<USMHealthComponent>("HealthComponent");
	WeaponComponent = CreateDefaultSubobject<USMWeaponComponent>("WeaponComponent");
	ElementComponent = CreateDefaultSubobject<USMElementComponent>("ElementComponent");

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);
}

void ASMCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	check(HealthComponent);
	check(DestructComponent);
	check(WeaponComponent);
	check(ElementComponent);

	OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnDamage);
	HealthComponent->OnDeath.AddUObject(this, &ThisClass::OnDeath);
}

void ASMCharacterBase::OnDeath()
{
	if(GetCharacterMovement()) GetCharacterMovement()->DisableMovement();
	if(GetCapsuleComponent()) GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DestructComponent->Destruct();
	WeaponComponent->DestroyComponent();
}

void ASMCharacterBase::OnDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	HealthComponent->TakeDamage(Damage * ElementComponent->GetDamageFactor(DamageType));
}
