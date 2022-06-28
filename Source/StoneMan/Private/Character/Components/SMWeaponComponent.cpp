// Created by DChepurkin

#include "Character/Components/SMWeaponComponent.h"

#include "SMWeaponBase.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

USMWeaponComponent::USMWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USMWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapons();
}

void USMWeaponComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
	DestroyWeapons();
}

void USMWeaponComponent::SpawnWeapons()
{
	for(const auto& [Element, WeaponData] : WeaponsData)
	{
		const auto WeaponClass = WeaponData.WeaponClass;
		const auto Weapon = SpawnNewWeapon(WeaponClass);
		if(Weapon) Weapons.Add(Element, Weapon);
	}
}

ASMWeaponBase* USMWeaponComponent::SpawnNewWeapon(const TSubclassOf<ASMWeaponBase> NewWeapon)
{
	if(!NewWeapon || !GetWorld()) return nullptr;

	const auto Character = GetOwner<ACharacter>();
	if(!Character) return nullptr;

	const auto Weapon = GetWorld()->SpawnActor<ASMWeaponBase>(NewWeapon);
	if(Weapon) Weapon->SetOwner(Character);
	AttachWeaponToComponent(Weapon, Character->GetMesh());

	return Weapon;
}

void USMWeaponComponent::AttachWeaponToComponent(ASMWeaponBase* Weapon, USceneComponent* AttachTo)
{
	if(!Weapon || !AttachTo) return;

	const auto Element = Weapon->GetElement();
	Weapon->AttachToComponent(AttachTo, {EAttachmentRule::SnapToTarget, true}, GetSocketName(Element));
}

const FName& USMWeaponComponent::GetSocketName(const ESMCharacterElement Element)
{
	return WeaponsData[Element].SocketName;
}

void USMWeaponComponent::DestroyWeapons()
{
	for(const auto& [Element, Weapon] : Weapons)
	{
		if(Weapon) Weapon->Destroy();
	}
}

void USMWeaponComponent::StartAttack(const ESMCharacterElement Element)
{
	UE_LOG(LogWeaponComponent, Display, TEXT("WEAPON COMPONENT"))
	if(Weapons.Contains(Element)) Weapons[Element]->StartAttack();
}
