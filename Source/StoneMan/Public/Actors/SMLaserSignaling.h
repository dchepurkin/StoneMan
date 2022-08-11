// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "SMCoreTypes.h"
#include "Actors/SMSwitchBase.h"
#include "SMLaserSignaling.generated.h"

class USMLaserComponent;

UCLASS(Blueprintable)
class STONEMAN_API ASMLaserSignaling : public ASMSwitchBase
{
	GENERATED_BODY()

public:
	ASMLaserSignaling();
	virtual void ResetSwitch() override;	

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	USMLaserComponent* LaserComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMSwitch)
	FLinearColor LaserColor = FLinearColor::Green;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category=SMSwitch)
	TArray<FSwitches> SwitchesesRules;

	virtual void BeginPlay() override;

private:
	void OnDetectCharacter();
	void BindSwitchesesDelegates();
	void OnChangeSwitch();
	bool CheckForOpen();
};
