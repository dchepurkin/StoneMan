// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "SMDoorBase.generated.h"

UCLASS(Abstract, NotBlueprintable)
class STONEMAN_API ASMDoorBase : public AActor
{
	GENERATED_BODY()

public:
	ASMDoorBase();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UStaticMeshComponent* ArchMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UTimelineComponent* OpenDoorTimeline;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMDoor)
	UCurveFloat* OpenDoorCurve;	

	virtual void BeginPlay() override;

	void OpenDoor() const;
	void CloseDoor() const;

private:
	FOnTimelineFloat OnOpenDoorTimelineFloat;

	UFUNCTION()
	virtual void OnOpenCloseDoor(const float Alpha) {}
};
