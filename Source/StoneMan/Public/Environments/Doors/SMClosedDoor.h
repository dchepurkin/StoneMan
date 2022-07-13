// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "Environments/Doors/SMDoorBase.h"
#include "SMClosedDoor.generated.h"

class ASMSwitchBase;

UENUM(BlueprintType)
enum class EOpenOn:uint8
{
	OpenOnSwitchON = 0,
	OpenOnSwitchOFF
};

USTRUCT(BlueprintType)
struct FSwitches
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ASMSwitchBase* Switch = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Open On Switch(ON/OFF)")
	EOpenOn OpenOn;

	bool SwitchIsOpen = false;
};

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
	void OnChangeSwitchEnabled(AActor* Switch, const bool Enabled);
	virtual void OnOpenCloseDoor(const float Alpha) override;

	bool CheckForOpen();
};
