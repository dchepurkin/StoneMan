// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SMPushComponent.generated.h"

class ASMPushableActor;

UCLASS(ClassGroup=(Custom), Blueprintable, DefaultToInstanced, BlueprintType, meta=(BlueprintSpawnableComponent))
class STONEMAN_API USMPushComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USMPushComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartPush();
	void StopPush();

protected:
	virtual void BeginPlay() override;

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
};
