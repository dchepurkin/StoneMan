// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "Actors/SMPushableActor.h"
#include "SMLaserCube.generated.h"

class USMLaserComponent;

UCLASS()
class STONEMAN_API ASMLaserCube : public ASMPushableActor
{
	GENERATED_BODY()

public:
	ASMLaserCube();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	USMLaserComponent* LaserComponent;

	virtual void BeginPlay() override;

private:
	void UpdateLaserColor() const;
};
