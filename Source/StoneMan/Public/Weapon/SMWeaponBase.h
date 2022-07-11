// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "SMCoreTypes.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "SMWeaponBase.generated.h"

class USMDamageTypeBase;

UCLASS()
class STONEMAN_API ASMWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	ASMWeaponBase();
	ESMCharacterElement GetElement() const { return Element; }
	void StartAttack();
	void Show(const bool Visibility) const;
	void RestorePower(const float PowerAmount);
	void MakeDamage() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UTimelineComponent* MaterialTimeline;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMWeapon)
	ESMCharacterElement Element = ESMCharacterElement::Ice;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMWeapon)
	TSubclassOf<USMDamageTypeBase> DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMWeapon|Damage", meta=(ClampMin = 1.f))
	float Damage = 15.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMWeapon|Damage", meta=(ClampMin = 100.f))
	float MaxPower = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMWeapon|Damage", meta=(ClampMin = 1.f))
	float AttackCoast = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMWeapon|Damage", meta=(ClampMin = 0.1f))
	float LowPowerAttackFactor = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMWeapon|Damage", meta=(ClampMin = 10.f))
	float DamageDistance = 120.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMWeapon|Damage", meta=(ClampMin = 10.f))
	float DamageRadius = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMWeapon|Damage")
	bool DoFullDamage = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMWeapon|Damage", DisplayName=DrawDebugSphere)
	bool bDrawDebugSphere = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMWeapon|Damage", meta=(EditCondition=bDrawDebugSphere))
	FColor DebugColor = FColor::Red;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMWeapon|Material")
	UCurveFloat* MaterialTimelineCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMWeapon|Material")
	FName DisolveParameterName = "Dissolve";

private:
	float CurrentPower = 0.f;
	FOnTimelineFloat MaterialTimelineDelegate;

	UFUNCTION()
	void OnMaterialTimeline(const float Alpha);

	UMaterialInstanceDynamic* GetMaterial() const;
	float GetPowerFactor() const { return CurrentPower >= AttackCoast ? 1.f : LowPowerAttackFactor; }

	void SetPower(const float NewPower);
};
