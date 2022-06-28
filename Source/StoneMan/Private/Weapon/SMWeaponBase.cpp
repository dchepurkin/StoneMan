// Created by DChepurkin

#include "Weapon/SMWeaponBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All);

ASMWeaponBase::ASMWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	SetRootComponent(StaticMesh);
}

void ASMWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASMWeaponBase::StartAttack()
{
	if(!CanAttack()) return;
	UE_LOG(LogWeapon, Display, TEXT("ATTACK"))
}
