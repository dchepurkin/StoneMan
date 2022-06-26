#pragma once

UENUM(BlueprintType)
enum class ESMPlayerState : uint8
{
	Idle = 0,
	Push,
	Dead,
};