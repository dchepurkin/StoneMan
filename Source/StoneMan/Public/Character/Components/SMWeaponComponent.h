// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "SMCoreTypes.h"
#include "Components/ActorComponent.h"
#include "SMWeaponComponent.generated.h"

class ASMWeaponBase;

DECLARE_MULTICAST_DELEGATE(FOnStartAttackSignature);
DECLARE_MULTICAST_DELEGATE(FOnEndAttackSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStartNextComboSectionSignature, const FName&);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STONEMAN_API USMWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USMWeaponComponent();
	void StartAttack(const ESMElement Element);
	void StopAttack();
	void SetNextComboSectionEnabled(const bool Enabled, const FName& NextSectionName = NAME_None);
	void ShowWeapon(const bool Visibility);
	void MakeDamage() const;

	FOnStartAttackSignature OnStartAttack;
	FOnEndAttackSignature OnEndAttack;
	FOnStartNextComboSectionSignature OnStartNextComboSection;

protected:
	virtual void BeginPlay() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMWeaponComponent, DisplayName="Weapons")
	TMap<ESMElement, FWeaponData> WeaponsData{
		{ESMElement::Ice, FWeaponData()},
		{ESMElement::Fire, FWeaponData()},
		{ESMElement::Earth, FWeaponData()},
		{ESMElement::Air, FWeaponData()}
	};
private:
	TMap<ESMElement, ASMWeaponBase*> Weapons;

	UPROPERTY()
	ASMWeaponBase* CurrentWeapon;

	bool AttackInProgress = false;
	bool NextComboSection = false;

	FName NextComboSectionName = NAME_None;

	void SpawnWeapons();
	void DestroyWeapons();
	void AttachWeaponToComponent(ASMWeaponBase* Weapon, USceneComponent* AttachTo);

	ASMWeaponBase* SpawnNewWeapon(const TSubclassOf<ASMWeaponBase> NewWeapon);
	const FName& GetSocketName(const ESMElement Element);
};
