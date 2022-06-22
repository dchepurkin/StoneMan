// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SMPushComponent.generated.h"

class ASMPushableActor;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STONEMAN_API USMPushComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USMPushComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	ASMPushableActor* CurrentPushable;
};
