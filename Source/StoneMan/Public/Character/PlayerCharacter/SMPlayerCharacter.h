// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "SMPlayerCharacterState.h"
#include "Character/SMCharacterBase.h"
#include "SMPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class STONEMAN_API ASMPlayerCharacter : public ASMCharacterBase
{
	GENERATED_BODY()

public:
	explicit ASMPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UCameraComponent* CameraComponent;

private:
	SMPlayerCharacterState PlayerCharacterState = SMPlayerCharacterState(this);
	
	void MoveForward(const float AxisValue);
	void MoveRight(const float AxisValue);
};
