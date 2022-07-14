// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "SMSwitchBase.h"
#include "SMSwitchPlane.generated.h"

class UBoxComponent;
class USMElementComponent;

UCLASS(Blueprintable)
class STONEMAN_API ASMSwitchPlane : public ASMSwitchBase
{
	GENERATED_BODY()

public:
	ASMSwitchPlane();
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Component)
	USMElementComponent* ElementComponent;

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

	virtual void SetSwitchEnabled(const bool Enabled) override;

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                    const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool SwitchCheck() const;
	bool LineTrace(const float X1, const float Y1, const float X2, const float Y2) const;
	bool CheckElement(AActor* Actor) const;
};
