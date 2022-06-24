// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	FOnStartPushSignature OnStartPush;
	FOnStopPushSignature OnStopPush;

	void StartPush();
	void StopPush();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMPushComponent, meta=(ClampMin = 0.1f))
	float PushRange = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMPushComponent, meta=(ClampMin = 0.75f, ClampMax = 1.f))
	float PushableAngle = 0.95f;

private:
	UPROPERTY()
	ASMPushableActor* CurrentPushable;

	FTimerHandle TryToPushTimerHandle;

	void TryToPush();
	bool LineTrace(FHitResult& HitResult) const;
	bool CanPush() const;
};
