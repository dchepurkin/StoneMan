// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "SMCameraComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCameraBeginOverlapSignature, AActor*)
DECLARE_MULTICAST_DELEGATE(FOnCameraEndOverlapSignature)

UCLASS()
class STONEMAN_API USMCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	USMCameraComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FOnCameraBeginOverlapSignature OnCameraBeginOverlap;
	FOnCameraEndOverlapSignature OnCameraEndOverlap;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMCameraComponent, meta=(ClampMin = 0.1f))
	float FadeDistance = 10.f;

private:
	bool IsOverlaped = false;
	void CheckCameraCollision();
};
