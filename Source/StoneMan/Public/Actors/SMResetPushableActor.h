// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "SMInteractionInterface.h"
#include "GameFramework/Actor.h"
#include "SMResetPushableActor.generated.h"

class ASMPushableActor;
class UTimelineComponent;

UCLASS()
class STONEMAN_API ASMResetPushableActor : public AActor, public ISMInteractionInterface
{
	GENERATED_BODY()

public:
	ASMResetPushableActor();

	virtual void Interact(AActor* WhoInteract) override;
	virtual const FString& GetInteractionMessage() const override { return InteractionMessage; }
	virtual void SetOutlineVisible(const bool Visible) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UTimelineComponent* Timeline;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UStaticMeshComponent* ArmMesh;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category=SMReseter)
	TArray<ASMPushableActor*> ActorsToReset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMReseter)
	UCurveFloat* TimelineCurve;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMReseter)
	float RotationAngle = 110.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMReseter)
	FString InteractionMessage = "Reset";

	virtual void BeginPlay() override;

private:
	FRotator StartRotation;
	FRotator EndRotation;
	
	UFUNCTION()
	void ResetActors();

	UFUNCTION()
	void OnTimeline(const float Alpha);
};
