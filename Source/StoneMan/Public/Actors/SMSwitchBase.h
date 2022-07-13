// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMSwitchBase.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChangeSwitchEnabledSignature, AActor*, const bool);

UCLASS(Abstract, NotBlueprintable)
class STONEMAN_API ASMSwitchBase : public AActor
{
	GENERATED_BODY()

public:
	FOnChangeSwitchEnabledSignature OnChangeSwitchEnabled;

	ASMSwitchBase();

protected:
	bool bSwitched = false;
	virtual void BeginPlay() override;
	virtual void SetSwitchEnabled(const bool Enabled);
};
