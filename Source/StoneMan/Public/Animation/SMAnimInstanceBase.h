// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
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
};
