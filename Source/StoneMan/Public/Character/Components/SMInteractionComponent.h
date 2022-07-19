// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SMInteractionComponent.generated.h"

class UBoxComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STONEMAN_API USMInteractionComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	USMInteractionComponent();
	void Interact();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Components)
	UBoxComponent* InteractionCollision;

	virtual void BeginPlay() override;

private:
	UPROPERTY()
	AActor* CurrentInteractionActor;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                    const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
