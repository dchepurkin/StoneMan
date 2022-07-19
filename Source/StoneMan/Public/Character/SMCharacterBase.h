// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SMCharacterBase.generated.h"

class UGeometryCollectionComponent;
class USMDestructComponent;
class USMHealthComponent;
class USMWeaponComponent;
class USMElementComponent;

UCLASS(Abstract)
class STONEMAN_API ASMCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	explicit ASMCharacterBase(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	USMDestructComponent* DestructComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	USMHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	USMWeaponComponent* WeaponComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Components)
	USMElementComponent* ElementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMCharacter)
	UAnimMontage* AttackAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMCharacter)
	FName StartComboSectionName = "0";

	virtual void OnDeath();
	void Attack();
	virtual bool CanAttack() const;
	virtual void OnStartAttack();
	virtual void OnEndAttack();

private:
	UFUNCTION()
	void OnDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
};
