// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SMMakeDamageNotify.generated.h"

class USMWeaponComponent;

UCLASS()
class STONEMAN_API USMMakeDamageNotify : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override { return FString("Make Damage"); }
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override;

private:
	USMWeaponComponent* GetWeaponComponent(USkeletalMeshComponent* MeshComp);
};
