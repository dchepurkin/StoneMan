// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "SMCoreTypes.h"
#include "Components/ActorComponent.h"
#include "SMHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float)
DECLARE_MULTICAST_DELEGATE(FOnDeathSignature)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STONEMAN_API USMHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USMHealthComponent();

	FOnHealthChangedSignature OnHealthChanged;
	FOnDeathSignature OnDeath;

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const { return CurrentHealth / MaxHealth; }

	void MakeHeal(const float HealAmout);
	void TakeDamage(const float DamageAmount);
	bool IsDead() const;

	bool IsFallingVelocityIsDamaged(const FVector& Velocity) const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMHealthComponent)
	float MaxHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMHealthComponent|AutoHeal")
	bool AutoHeal = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMHealthComponent|AutoHeal", meta=(EditCondition = AutoHeal, ClampMin = 0.f))
	float AutoHealDelay = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMHealthComponent|AutoHeal", meta=(EditCondition = AutoHeal, ClampMin = 0.001f))
	float AutoHealRate = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMHealthComponent|AutoHeal", meta=(EditCondition = AutoHeal, ClampMin = 0.001f))
	float AutoHealAmount = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMHealthComponent|OnLanded")
	bool LandedDamage = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMHealthComponent|OnLanded", meta=(EditCondition = LandedDamage))
	FVector2D LandedDamageVelocity = FVector2D(900.f, 1100.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMHealthComponent|OnLanded", meta=(EditCondition = LandedDamage))
	FVector2D LandedDamageAmount = FVector2D(10.f, 100.f);

private:
	float CurrentHealth = 0.f;
	FTimerHandle AutoHealTimerHandle;

	void SetHealth(const float NewHealth);	

	UFUNCTION()
	void OnLanded(const FHitResult& Hit);

	void OnAutoHeal();
	void SetAutoHealTimerEnabled(const bool Enabled);
};
