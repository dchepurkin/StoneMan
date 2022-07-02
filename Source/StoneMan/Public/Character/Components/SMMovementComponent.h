// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SMMovementComponent.generated.h"

UCLASS()
class STONEMAN_API USMMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	USMMovementComponent();
	virtual float GetMaxSpeed() const override;
	void SetSprintEnabled(const bool Enabled) { bSprint = Enabled; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMMovement)
	float SprintMaxSpeed = 900.f;

private:
	bool bSprint = false;
};
