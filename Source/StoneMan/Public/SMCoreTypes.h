#pragma once

#include "SMCoreTypes.generated.h"

class ASMWeaponBase;
class ASMSwitchBase;

UENUM(BlueprintType)
enum class ESMPlayerState : uint8
{
	Idle = 0,
	Push,
	Attack,
	ChangeElement,
};

UENUM(BlueprintType)
enum class ESMElement : uint8
{
	Ice = 0,
	Fire,
	Air,
	Earth,
	NoElement
};

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ASMWeaponBase> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SocketName = NAME_None;
};

UENUM(BlueprintType)
enum class EOpenOn:uint8
{
	OpenOnSwitchON = 0,
	OpenOnSwitchOFF
};

USTRUCT(BlueprintType)
struct FSwitches
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ASMSwitchBase* Switch = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Open On Switch(ON/OFF)")
	EOpenOn OpenOn = EOpenOn::OpenOnSwitchON;
};
