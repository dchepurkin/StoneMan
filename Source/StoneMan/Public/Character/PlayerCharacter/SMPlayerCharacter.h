// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "SMCoreTypes.h"
#include "Character/SMCharacterBase.h"
#include "SMPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USMPushComponent;

UCLASS()
class STONEMAN_API ASMPlayerCharacter : public ASMCharacterBase
{
	GENERATED_BODY()

public:
	explicit ASMPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UCameraComponent* CameraComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
    USMPushComponent* PushComponent;

private:
	ESMPlayerState PlayerState = ESMPlayerState::Idle;

	void MoveForward(float AxisValue);
	void MoveRight(const float AxisValue);

	virtual void Jump() override;

	void SetState(const ESMPlayerState NewState) { PlayerState = NewState; }
};
