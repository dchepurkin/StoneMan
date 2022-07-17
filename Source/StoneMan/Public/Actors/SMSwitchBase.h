// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMSwitchBase.generated.h"

class USoundCue;

DECLARE_MULTICAST_DELEGATE(FOnChangeSwitchEnabledSignature);

UCLASS(Abstract, NotBlueprintable)
class STONEMAN_API ASMSwitchBase : public AActor
{
	GENERATED_BODY()

public:
	FOnChangeSwitchEnabledSignature OnChangeSwitchEnabled;

	ASMSwitchBase();
	bool IsEnabled() const { return bSwitched; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMSwitch|Sounds")
	USoundCue* SwitchONSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMSwitch|Sounds")
	USoundCue* SwitchOFFSound;

	bool bSwitched = false;
	virtual void BeginPlay() override;
	virtual void SetSwitchEnabled(const bool Enabled);
};
