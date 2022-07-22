// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "SMInteractionInterface.h"
#include "SMLaserInterface.h"
#include "Actors/SMPushableActor.h"
#include "Components/BoxComponent.h"
#include "SMLaserCube.generated.h"

class USMLaserComponent;

UCLASS()
class STONEMAN_API ASMLaserCube : public ASMPushableActor, public ISMInteractionInterface, public ISMLaserInterface
{
	GENERATED_BODY()

public:
	ASMLaserCube();
	virtual void Interact(AActor* WhoInteract) override;
	virtual void SetOutlineVisible(const bool Visible) override;
	virtual const FString& GetInteractionMessage() const override { return InteractionMessage; }
	virtual void UpdateColor() override;

	virtual void Activate(const ESMElement Element) override;
	virtual void Disactivate() override;

	virtual void ResetCondition() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	USMLaserComponent* LaserComponent;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMPushableActor)
	FString InteractionMessage = "Switch";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UBoxComponent* XLaserTrigger;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UBoxComponent* YLaserTrigger;

	virtual void BeginPlay() override;

private:
	void UpdateLaserColor() const;
	void CreateLaserTrigger(UBoxComponent*& LaserTrigger, const FName& TriggerName);
	void OnDetectLaserTrigger(AActor* Actor);
	void OnLoseLaserTrigger(AActor* Actor);
};
