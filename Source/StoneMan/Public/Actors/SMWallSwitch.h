// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "SMInteractionInterface.h"
#include "Actors/SMSwitchBase.h"
#include "Components/TimelineComponent.h"
#include "SMWallSwitch.generated.h"

UCLASS(Blueprintable)
class STONEMAN_API ASMWallSwitch : public ASMSwitchBase, public ISMInteractionInterface
{
	GENERATED_BODY()
public:
	ASMWallSwitch();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Interact(AActor* WhoInteract) override;
	virtual void SetOutlineVisible(const bool Visible) override;

	UFUNCTION(BlueprintCallable)
	virtual FString GetInteractionMessage() const override { return bSwitched ? InteractionMessageOFF : InteractionMessageON; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UTimelineComponent* SwitchTimeline;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UStaticMeshComponent* ArmMesh;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category=SMSwitch)
	bool IsEnabled = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMSwitch, meta=(ClampMin = 0.f))
	float SwitchAngle = 110.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMSwitch|Messages")
	FString InteractionMessageON = TEXT("Включить");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMSwitch|Messages")
	FString InteractionMessageOFF = TEXT("Выключить");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMSwitch|LightColor")
	FName LightColorParameterName = "LightColor";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMSwitch|LightColor")
	FLinearColor ONColor = FLinearColor::Green;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMSwitch|LightColor")
	FLinearColor OFFColor = FLinearColor::Red;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMSwitch)
	UCurveFloat* SwitchCurve;

	virtual void BeginPlay() override;

private:
	FRotator EnabledRotation;
	FRotator DisabledRotation;
	FOnTimelineFloat OnSwitchTimeline;
	FOnTimelineEvent OnSwitchTimelineFinished;

	UFUNCTION()
	void OnSwitchTimelineCallback(float Alpha);

	UFUNCTION()
	void OnSwitchTimelineFinishedCallback();

	void SetLightColor(const FLinearColor& NewColor) const;
	void SetSwitchRotation(const FRotator& NewRotation) const;
};
