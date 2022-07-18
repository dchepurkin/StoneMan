// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SMLaserComponent.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STONEMAN_API USMLaserComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	USMLaserComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetLaserColor(const FLinearColor& NewColor) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMLaserComponent)
	UNiagaraSystem* LaserNiagaraSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMLaserComponent)
	FString LaserEndParameterName = "LaserEnd";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMLaserComponent)
	FString LaserColorParameterName = "LaserColor";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMLaserComponent, meta=(ClampMin = 100.f))
	float LaserMaxLength = 2000.f;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UNiagaraComponent* Laser;

	const FVector& GetLaserEnd() const;
};
