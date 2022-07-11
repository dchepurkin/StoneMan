// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "SMCoreTypes.h"
#include "Character/SMCharacterBase.h"
#include "SMPlayerCharacter.generated.h"

class USpringArmComponent;
class USMCameraComponent;
class USMPushComponent;
class USMStateComponent;

UCLASS()
class STONEMAN_API ASMPlayerCharacter : public ASMCharacterBase
{
	GENERATED_BODY()

public:
	explicit ASMPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetState(const ESMPlayerState NewState) { PlayerState = NewState; }
	ESMPlayerState GetState() const { return PlayerState; }
	void ChangeColor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	USMCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	USMPushComponent* PushComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMCharacter)
	UAnimMontage* ChangeElementAnimMontage;

	virtual void OnDeath() override;
	virtual bool CanAttack() const override;
	virtual void OnStartAttack() override;
	virtual void OnEndAttack() override;

private:
	ESMPlayerState PlayerState = ESMPlayerState::Idle;

	void MoveForward(float AxisValue);
	void MoveRight(const float AxisValue);
	virtual void Jump() override;

	void OnStartPush();
	void OnStopPush();

	void OnCameraBeginOverlap(AActor* Actor);
	void OnCameraEndOverlap();

	void OnStartNextComboSection(const FName& NextComboSectionName);
	void SetNextComboSection(const FName& NextComboSectionName) const;

	void SetMeshVisibility(const bool Enabled);

	void OnTryToSetElement(const ESMCharacterElement NewElement);
	void OnChangeElement();
};
