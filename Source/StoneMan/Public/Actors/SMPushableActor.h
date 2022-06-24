// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "SMPushComponent.h"
#include "GameFramework/Actor.h"
#include "SMPushableActor.generated.h"

class UBoxComponent;

UCLASS()
class STONEMAN_API ASMPushableActor : public AActor
{
	GENERATED_BODY()

public:
	ASMPushableActor();
	virtual void Tick(float DeltaTime) override;

	const FTransform& GetClosestPushTransform(const AActor* PushingActor);
	bool CanMove();

	void StartPush(const FVector& NewPushVector);
	void StopPush();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMPushableActor)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMPushableActor)
	UBoxComponent* XAxisCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMPushableActor)
	UBoxComponent* YAxisCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMPushableActor)
	UBoxComponent* XEndAxisCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMPushableActor)
	UBoxComponent* YEndAxisCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SMPushableActor, meta=(MakeEditWidget=true))
	TArray<FTransform> PushTransforms;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=SMPushableActor,meta=(ClampMin = 0.f))
	float PushSpeed = 100.f;
	
private:
	bool IsPlayerCharacterOverlaped = false;
	bool IsMoving = false;
	FVector PushVector = FVector::ZeroVector;

	UFUNCTION()
	void OnAxisCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                                 bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnAxisCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnEndAxisCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool IsActorHavePushComponent(AActor* Actor);
};
