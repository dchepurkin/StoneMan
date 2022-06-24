// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SMCoreTypes.h"
#include "SMAnimInstanceBase.generated.h"

class ASMCharacterBase;

UCLASS()
class STONEMAN_API USMAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	ASMCharacterBase* Character;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=SMAnimation)
	float Speed = 0.f;

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
