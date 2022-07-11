// Created by DChepurkin

#include "Weapon/SMWeaponBase.h"
#include "Weapon/DamageType/SMDamageTypeBase.h"

#include "Kismet/GameplayStatics.h"

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

	SetPower(MaxPower);
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

void ASMWeaponBase::StartAttack() { }//TODO удалить если не используется

void ASMWeaponBase::Show(const bool Visibility) const
{
	Visibility
		? MaterialTimeline->Play()
		: MaterialTimeline->Reverse();
}

void ASMWeaponBase::RestorePower(const float PowerAmount)
{
	if(PowerAmount <= 0.f) return;
	SetPower(CurrentPower + PowerAmount);
}

void ASMWeaponBase::SetPower(const float NewPower)
{
	CurrentPower = FMath::Clamp<float>(NewPower, 0.f, MaxPower);
}

void ASMWeaponBase::MakeDamage() const
{
	const auto Pawn = GetOwner<APawn>();
	if(!GetOwner()) return;

	const auto Controller = Pawn->GetController();
	if(!Controller) return;

	const auto DamageDirection = Pawn->GetActorForwardVector() * (DamageDistance);
	const auto DamagePoint = Pawn->GetActorLocation() + DamageDirection;

	UGameplayStatics::ApplyRadialDamage(this, Damage, DamagePoint, DamageRadius, DamageType, {Pawn}, Pawn, Controller, DoFullDamage);

	if(bDrawDebugSphere && GetWorld())
	{
		DrawDebugSphere(GetWorld(), DamagePoint, DamageRadius, 24, DebugColor, false, 2.f, 0, 1.5f);
	}
}
