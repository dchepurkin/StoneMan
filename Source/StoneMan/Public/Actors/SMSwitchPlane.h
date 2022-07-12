// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMSwitchPlane.generated.h"

class UBoxComponent;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChangeSwitchEnabledSignature, AActor*, const bool);

UCLASS()
class STONEMAN_API ASMSwitchPlane : public AActor
{
	GENERATED_BODY()

public:
	FOnChangeSwitchEnabledSignature OnChangeSwitchEnabled;
	
	ASMSwitchPlane();
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Component)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Component)
	UBoxComponent* BoxCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Component)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SMSwitch", meta=(ClampMin = 0.f))
	float SwitchedZOffset = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SMSwitch|Debug", DisplayName="Draw Debug Lines")
	bool bDrawDebugLines = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SMSwitch|Debug", meta=(EditCondition=bDrawDebugLines))
	FColor DebugColor = FColor::Green;

private:
	bool bSwitched = false;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                    const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool SwitchCheck() const;
	bool LineTrace(const float X1, const float Y1, const float X2, const float Y2) const;

	void SetSwitchEnabled(const bool Enabled);
};
