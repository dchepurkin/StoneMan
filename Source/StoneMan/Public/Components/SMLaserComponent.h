// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SMLaserComponent.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;

UENUM(BlueprintType)
enum class ESMLaserDirestion : uint8
{
	Forward,
	Backward,
	Left,
	Right
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDetectLaserTriggerSignature, AActor*)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLoseLaserTriggerSignature, AActor*)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STONEMAN_API USMLaserComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	FOnDetectLaserTriggerSignature OnDetectLaserTrigger;
	FOnLoseLaserTriggerSignature OnLoseLaserTrigger;

	USMLaserComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetLaserColor(const FLinearColor& NewColor) const;
	void RotateLaser();
	void SetLaserEnabled(const bool Enabled);
	bool IsEnabled() const { return bEnabled; }
	bool IsUndisablable() const {return Undisabable;}

	UFUNCTION(BlueprintCallable)
	const FVector& GetConstructionLaserDirection() const { return LaserDirectionMap[LaserDirestion]; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMLaserComponent)
	UNiagaraSystem* LaserNiagaraSystem;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category=SMLaserComponent, DisplayName=Enabled)
	bool bEnabled = false;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category=SMLaserComponent)
	ESMLaserDirestion LaserDirestion = ESMLaserDirestion::Forward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMLaserComponent)
	FString LaserEndParameterName = "LaserEnd";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMLaserComponent)
	FString LaserColorParameterName = "LaserColor";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMLaserComponent, meta=(ClampMin = 100.f))
	float LaserMaxLength = 3000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMLaserComponent)
	FName LaserTriggerTag = "LaserTrigger";

	virtual void BeginPlay() override;

private:
	bool Undisabable = false;
	FVector CurrentLaserDirection;

	UPROPERTY()
	AActor* CurrentNextLaser;

	UPROPERTY()
	UNiagaraComponent* Laser;

	TMap<ESMLaserDirestion, FVector> LaserDirectionMap
	{
		{ESMLaserDirestion::Forward, FVector(1.f, 0.f, 0.f)},
		{ESMLaserDirestion::Backward, FVector(-1.f, 0.f, 0.f)},
		{ESMLaserDirestion::Right, FVector(0.f, 1.f, 0.f)},
		{ESMLaserDirestion::Left, FVector(0.f, -1.f, 0.f)}
	};
	
	void TryToDetectLaserTrigger(const FHitResult& HitResult);
	void ClearNextLaser();
	void MakeLaser(FVector& LaserEnd, FHitResult& HitResult) const;
	bool CheckForPriviewLaser(const FHitResult& HitResult) const;
};
