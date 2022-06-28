#pragma once

#include "SMCoreTypes.generated.h"

class ASMWeaponBase;

UENUM(BlueprintType)
enum class ESMPlayerState : uint8
{
	Idle = 0,
	Push,
};

UENUM(BlueprintType)
enum class ESMCharacterElement : uint8
{
	Ice = 0,
	Fire,
	Air,
	Earth,
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
