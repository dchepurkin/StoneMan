// Created by DChepurkin

#include "Character/Components/SMHealthComponent.h"

#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

USMHealthComponent::USMHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USMHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHealth(MaxHealth);

	if(const auto Character = GetOwner<ACharacter>())
	{
		Character->OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnTakeAnyDamage);
		if(LandedDamage) Character->LandedDelegate.AddDynamic(this, &ThisClass::OnLanded);
	}
}

void USMHealthComponent::SetHealth(const float NewHealth)
{
	CurrentHealth = FMath::Clamp<float>(NewHealth, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth);
}

void USMHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
                                         AActor* DamageCauser)
{
	if(Damage <= 0.f || IsDead()) return;

	SetAutoHealTimerEnabled(false);

	SetHealth(CurrentHealth - Damage);
	if(IsDead()) OnDeath.Broadcast();

	SetAutoHealTimerEnabled(true);
}

void USMHealthComponent::OnAutoHeal()
{
	MakeHeal(AutoHealAmount);
	if(FMath::IsNearlyEqual(CurrentHealth, MaxHealth)) SetAutoHealTimerEnabled(false);
}

void USMHealthComponent::MakeHeal(const float HealAmout)
{
	if(HealAmout > 0) SetHealth(CurrentHealth + HealAmout);
}

bool USMHealthComponent::IsDead() const
{
	return FMath::IsNearlyZero(CurrentHealth);
}

bool USMHealthComponent::IsFallingVelocityIsDamaged(const FVector& Velocity) const
{
	return -Velocity.Z >= LandedDamageVelocity.X;
}

void USMHealthComponent::SetAutoHealTimerEnabled(const bool Enabled)
{
	if(IsDead() || !GetWorld() || !AutoHeal) return;

	Enabled
		? GetWorld()->GetTimerManager().SetTimer(AutoHealTimerHandle, this, &ThisClass::OnAutoHeal, AutoHealRate, true, AutoHealDelay)
		: GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);
}

void USMHealthComponent::OnLanded(const FHitResult& Hit)
{
	if(!GetOwner()) return;

	const auto Velocity = GetOwner()->GetVelocity();
	if(!IsFallingVelocityIsDamaged(Velocity)) return;

	const auto Damage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamageAmount, -Velocity.Z);
	GetOwner()->TakeDamage(Damage, FDamageEvent(), nullptr, nullptr);
}
