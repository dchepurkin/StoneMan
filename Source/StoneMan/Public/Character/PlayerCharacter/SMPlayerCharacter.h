// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "SMCoreTypes.h"
#include "Character/SMCharacterBase.h"
#include "SMPlayerCharacter.generated.h"

class USpringArmComponent;
class USMCameraComponent;
class USMPushComponent;

UCLASS()
class STONEMAN_API ASMPlayerCharacter : public ASMCharacterBase
{
	GENERATED_BODY()

public:
	explicit ASMPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	ESMPlayerState GetState() const { return PlayerState; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	USMCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	USMPushComponent* PushComponent;

	virtual void OnDeath() override;

private:
	ESMPlayerState PlayerState = ESMPlayerState::Idle;

	void MoveForward(float AxisValue);
	void MoveRight(const float AxisValue);

	void OnStartPush();
	void OnStopPush();

	virtual void Jump() override;

	void SetState(const ESMPlayerState NewState) { PlayerState = NewState; }
};
