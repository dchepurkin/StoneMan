// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SMLaserComponent.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class ASMCharacterBase;

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
DECLARE_MULTICAST_DELEGATE(FOnDetectCharacterSignature)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STONEMAN_API USMLaserComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	FOnDetectLaserTriggerSignature OnDetectLaserTrigger;
	FOnLoseLaserTriggerSignature OnLoseLaserTrigger;
	FOnDetectCharacterSignature OnDetectCharacter;

	USMLaserComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetLaserColor(const FLinearColor& NewColor) const;
	void RotateLaser();
	void SetLaserEnabled(const bool Enabled);
	void ResetDirection();
	void SetLaserDirection(const FVector& NewDirection) { CurrentLaserDirection = NewDirection; }
	bool IsEnabled() const { return bEnabled; }
	bool IsUndisablable() const { return Undisabable; }
	void SetReflect(const bool NewReflect) { CanReflect = NewReflect; }

	UFUNCTION(BlueprintCallable)
	const FVector& GetConstructionLaserDirection() const { return LaserDirectionMap[LaserDirestion]; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMLaserComponent)
	UNiagaraSystem* LaserNiagaraSystem;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category=SMLaserComponent, DisplayName=Enabled)
	bool bEnabled = false;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category=SMLaserComponent)
	ESMLaserDirestion LaserDirestion = ESMLaserDirestion::Forward;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="SMLaserComponent|Damage")
	bool IsDamaged = false;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="SMLaserComponent|Damage", meta=(ClampMin = 0.f, EditCondition=IsDamaged))
	float Damage = 0.5f;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="SMLaserComponent|Damage", meta=(ClampMin = 0.001f, EditCondition=IsDamaged))
	float DamageRate = 0.01f;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="SMLaserComponent|Damage", meta=(EditCondition=IsDamaged))
	FLinearColor DamageColor = FLinearColor(1.f, 0.f, 0.759645f);

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
	bool CanReflect = true;

	FTimerHandle DamageTimerHandle;

	FVector CurrentLaserDirection;

	UPROPERTY()
	AActor* CurrentLaserTrigger;

	UPROPERTY()
	AActor* CurrenDamagedActor;

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
	void ClearLaserTrigger();
	void MakeLaser(FVector& LaserEnd, FHitResult& HitResult) const;
	bool CheckForPriviewLaser(const FHitResult& HitResult) const;
	bool CheckForCharacter(const FHitResult& HitResult) const;

	void MakeDamage(AActor* DamagedActor) const;
	void SetDamageTimerEnabled(AActor* DamagedActor);
};
