// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SMMovementComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStaminaChangedSignature, float)

UCLASS()
class STONEMAN_API USMMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	FOnStaminaChangedSignature OnStaminaChanged;

	USMMovementComponent();
	virtual void BeginPlay() override;

	virtual float GetMaxSpeed() const override;
	void SetSprintEnabled(const bool Enabled);
	void StopStaminaChange();

	UFUNCTION(BlueprintCallable)
	float GetStaminaPercent() const { return CurrentStamina / MaxStamina; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMMovement)
	float WalkMaxSpeed = 150.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMMovement)
	float SprintMaxSpeed = 900.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMMovement, meta=(ClampMin = 0.1f))
	float MaxStamina = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMMovement, meta=(ClampMin = 0.01f))
	float SprintCoast = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMMovement, meta=(ClampMin = 0.01f))
	float StaminaUseRate = 0.01f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMMovement, meta=(ClampMin = 0.f))
	float StaminaRestoreDelay = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMMovement, meta=(ClampMin = 0.01f))
	float StaminaRestoreRate = 0.01f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMMovement, meta=(ClampMin = 0.01f))
	float StaminaRestoreAmount = 0.05f;

private:
	bool bSprint = false;
	float CurrentStamina;
	FTimerHandle StaminaRestoreTimerHandle;
	FTimerHandle StaminaUseTimerHandle;

	void SetStaminaUseTimerEnabled(const bool Enabled);
	void SetStaminaRestoreTimerEnabled(const bool Enabled);

	void SetStamina(const float NewStamina);
	void OnRestoreStamina();
	void OnSprint();

	bool CanSprint() const { return CurrentStamina >= SprintCoast; }
};
