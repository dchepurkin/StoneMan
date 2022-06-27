// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "SMCameraComponent.generated.h"

UCLASS()
class STONEMAN_API USMCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	USMCameraComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMCameraComponent, meta=(ClampMin = 0.1f))
	float FadeDistance = 10.f;

private:
	bool IsOnFadeDistance() const;
	void SetOwnerMeshVisibilityEnabled(const bool Enabled) const;
};
