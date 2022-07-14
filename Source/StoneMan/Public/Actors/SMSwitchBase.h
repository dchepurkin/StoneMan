// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMSwitchBase.generated.h"

class USoundCue;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChangeSwitchEnabledSignature, AActor*, const bool);

UCLASS(Abstract, NotBlueprintable)
class STONEMAN_API ASMSwitchBase : public AActor
{
	GENERATED_BODY()

public:
	FOnChangeSwitchEnabledSignature OnChangeSwitchEnabled;

	ASMSwitchBase();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMSwitch)
	USoundCue* SwitchONSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMSwitch)
	USoundCue* SwitchOFFSound;
	
	bool bSwitched = false;
	virtual void BeginPlay() override;
	virtual void SetSwitchEnabled(const bool Enabled);
};
