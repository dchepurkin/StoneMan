// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "SMCoreTypes.h"
#include "Components/ActorComponent.h"
#include "SMWeaponComponent.generated.h"

class ASMWeaponBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STONEMAN_API USMWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USMWeaponComponent();
	void StartAttack(const ESMCharacterElement Element);

protected:
	virtual void BeginPlay() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMWeaponComponent, DisplayName="Weapons")
	TMap<ESMCharacterElement, FWeaponData> WeaponsData{
		{ESMCharacterElement::Ice, FWeaponData()},
		{ESMCharacterElement::Fire, FWeaponData()},
		{ESMCharacterElement::Earth, FWeaponData()},
		{ESMCharacterElement::Air, FWeaponData()}
	};
private:
	TMap<ESMCharacterElement, ASMWeaponBase*> Weapons;

	void SpawnWeapons();
	ASMWeaponBase* SpawnNewWeapon(const TSubclassOf<ASMWeaponBase> NewWeapon);
	void AttachWeaponToComponent(ASMWeaponBase* Weapon, USceneComponent* AttachTo);
	const FName& GetSocketName(const ESMCharacterElement Element);
	void DestroyWeapons();
};
