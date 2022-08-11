// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "SMInteractionInterface.h"
#include "GameFramework/Actor.h"
#include "SMResetPushableActor.generated.h"

class ASMSwitchBase;
class USoundCue;
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

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category=SMReseter)
	TArray<ASMSwitchBase*> SwitchesToReset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMReseter)
	UCurveFloat* TimelineCurve;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMReseter)
	float RotationAngle = 60.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMReseter)
	FString InteractionMessage = "Reset";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMReseter|Sounds")
	USoundCue* StartSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMReseter|Sounds")
	USoundCue* MidleSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMReseter|Sounds")
	USoundCue* EndSound;

	virtual void BeginPlay() override;

private:
	bool bGoBack = false;
	FRotator StartRotation;
	FRotator EndRotation;
	
	void ResetActors();
	void PlaySound(USoundCue* Sound) const;

	UFUNCTION()
	void OnTimeline(const float Alpha);

	UFUNCTION()
	void OnTimelineEnd();
};
