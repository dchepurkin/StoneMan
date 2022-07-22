// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "SMCoreTypes.h"
#include "Environments/Doors/SMDoorBase.h"
#include "SMClosedDoor.generated.h"

UCLASS(Blueprintable)
class STONEMAN_API ASMClosedDoor : public ASMDoorBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category=SMDoor)
	TArray<FSwitches> SwitchesesRules;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMDoor, meta=(ClampMin = 0.f))
	float OpenZOffset = 180.f;

private:
	FVector ClosePosition = FVector::ZeroVector;
	FVector OpenPosition = FVector::ZeroVector;

	void BindSwitchesesDelegates();
	void OnChangeSwitch();
	virtual void OnOpenCloseDoor(const float Alpha) override;

	bool CheckForOpen();
};
