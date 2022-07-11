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
	if(!Weapons.Contains(Element)) return;

	CurrentWeapon = Weapons[Element];
	CurrentWeapon->StartAttack();
	if(!AttackInProgress)
	{
		OnStartAttack.Broadcast();
		AttackInProgress = true;
	}
	else if(AttackInProgress && NextComboSection)
	{
		OnStartNextComboSection.Broadcast(NextComboSectionName);
	}
}

void USMWeaponComponent::StopAttack()
{
	AttackInProgress = false;
	OnEndAttack.Broadcast();
}

void USMWeaponComponent::SetNextComboSectionEnabled(const bool Enabled, const FName& NextSectionName)
{
	NextComboSection = Enabled;
	NextComboSectionName = NextSectionName;
}

void USMWeaponComponent::ShowWeapon(const bool Visibility)
{
	if(!CurrentWeapon) return;

	CurrentWeapon->Show(Visibility);
}

void USMWeaponComponent::MakeDamage() const
{
	if(CurrentWeapon) CurrentWeapon->MakeDamage();
}
