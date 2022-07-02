// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "SMPushComponent.generated.h"

class ASMPushableActor;

DECLARE_MULTICAST_DELEGATE(FOnStartPushSignature)
DECLARE_MULTICAST_DELEGATE(FOnStopPushSignature)

UCLASS(ClassGroup=(Custom), Blueprintable, DefaultToInstanced, BlueprintType, meta=(BlueprintSpawnableComponent))
class STONEMAN_API USMPushComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USMPushComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FOnStartPushSignature OnStartPush;
	FOnStopPushSignature OnStopPush;

	void StartPush();
	void StopPush();
	void RestartPush();
	void TryToPush();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMPushComponent, meta=(ClampMin = 0.1f))
	float PushRange = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMPushComponent, meta=(ClampMin = 0.75f, ClampMax = 1.f))
	float PushableAngle = 0.95f;

private:
	UPROPERTY()
	ASMPushableActor* CurrentPushable;

	bool LineTrace(FHitResult& HitResult) const;
	bool CanPush(ASMPushableActor* PushableActor) const;
	bool IsOwnerFalling() const;
	bool IsOwnerIdle() const;
};
