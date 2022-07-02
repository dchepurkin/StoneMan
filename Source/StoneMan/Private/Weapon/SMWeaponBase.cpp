// Created by DChepurkin

#include "Weapon/SMWeaponBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All);

ASMWeaponBase::ASMWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	MaterialTimeline = CreateDefaultSubobject<UTimelineComponent>("MaterialTimeline");
	
	SetRootComponent(StaticMesh);
}

void ASMWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	MaterialTimelineDelegate.BindDynamic(this, &ThisClass::OnMaterialTimeline);
	if(MaterialTimelineCurve) MaterialTimeline->AddInterpFloat(MaterialTimelineCurve, MaterialTimelineDelegate);
}

void ASMWeaponBase::OnMaterialTimeline(const float Alpha)
{
	const auto MaterialInstance = GetMaterial();
	if(!MaterialInstance) return;

	MaterialInstance->SetScalarParameterValue(DisolveParameterName, Alpha);
}

UMaterialInstanceDynamic* ASMWeaponBase::GetMaterial() const
{
	if(!StaticMesh) return nullptr;

	return StaticMesh->CreateDynamicMaterialInstance(0);
}

void ASMWeaponBase::StartAttack()
{
	if(!CanAttack()) return;
}

void ASMWeaponBase::Show(const bool Visibility)
{
	Visibility
		? MaterialTimeline->Play()
		: MaterialTimeline->Reverse();
}
