// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "SMLaserInterface.h"
#include "SMCoreTypes.h"
#include "GameFramework/Actor.h"
#include "SMWallLaser.generated.h"

class USMElementComponent;
class USMLaserComponent;
class UBoxComponent;

UCLASS()
class STONEMAN_API ASMWallLaser : public AActor, public ISMLaserInterface
{
	GENERATED_BODY()

public:
	ASMWallLaser();

	virtual void Activate(const ESMElement Element) override;
	virtual void Disactivate() override;

	virtual void OnConstruction(const FTransform& Transform) override;
	void SetNextLaserEnabled(const bool Enabled) const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	USMElementComponent* ElementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	USMLaserComponent* LaserComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UBoxComponent* LaserTrigger;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category=SMWallLaser)
	ASMWallLaser* NextWallLaser;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category=SMWallLaser)
	TArray<FSwitches> SwitchesesRules;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMWallLaser|Emissive")
	FName EmissiveParameterName = "Emissive";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMWallLaser|Emissive", meta=(ClampMin=0.f))
	float EmissiveON = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SMWallLaser|Emissive", meta=(ClampMin=0.f))
	float EmissiveOFF = 0.01f;

private:
	void OnDetectLaserTrigger(AActor* Actor);
	void OnLoseLaserTrigger(AActor* Actor);

	void UpdateColor() const;
	void UpdateLaserColor() const;

	void BindSwitchesesDelegates();
	void OnChangeSwitch();
	bool CheckForOpen();
};
