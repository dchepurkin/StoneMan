// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "SMLaserInterface.h"
#include "Actors/SMSwitchBase.h"
#include "SMLaserSwitch.generated.h"

class USMElementComponent;
class UBoxComponent;

UCLASS(Blueprintable)
class STONEMAN_API ASMLaserSwitch : public ASMSwitchBase, public ISMLaserInterface
{
	GENERATED_BODY()

public:
	ASMLaserSwitch();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Activate(const ESMElement Element) override;
	virtual void Disactivate() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UStaticMeshComponent* StaticMech;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UBoxComponent* LaserTrigger;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	USMElementComponent* ElementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMSwitch, meta=(ClampMin = 0.001f))
	float EmissiveON = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMSwitch, meta=(ClampMin = 0.001f))
	float EmissiveOFF = 0.01f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMSwitch)
	FName EmissiveParamName = "Emissive";

	virtual void BeginPlay() override;
	virtual void SetSwitchEnabled(const bool Enabled) override;

private:
	void UpdateEmissive() const;
	void SetElement() const;
};
