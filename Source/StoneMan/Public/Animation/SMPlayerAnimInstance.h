// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "SMCoreTypes.h"
#include "Animation/SMAnimInstanceBase.h"
#include "SMPlayerAnimInstance.generated.h"

UCLASS()
class STONEMAN_API USMPlayerAnimInstance : public USMAnimInstanceBase
{
	GENERATED_BODY()
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=SMAnimation)
	float LeanAxis = 0.f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=SMAnimation)
	bool IsFalling = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=SMAnimation)
	bool IsMakeSecondJump = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=SMAnimation)
	ESMPlayerState PlayerState;

private:
	bool CanMakeSecondJump = true;
	bool TryToMakeSecondJump();
	ESMPlayerState GetPlayerCharacterState() const;
	
};
