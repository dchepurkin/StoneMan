// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMPushableActor.generated.h"

UCLASS()
class STONEMAN_API ASMPushableActor : public AActor
{
	GENERATED_BODY()

public:
	ASMPushableActor();
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	const FTransform& GetClosestPushTransform(const AActor* PushingActor);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMPushableActor)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SMPushableActor, meta=(MakeEditWidget=true))
	TArray<FTransform> PushTransforms;

private:
	void AddArrowsComponents();
};
