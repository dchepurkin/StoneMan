// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SMWeaponVisibilityNotify.generated.h"

UCLASS()
class STONEMAN_API USMWeaponVisibilityNotify : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override { return FString("WeaponVisibility"); }

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category=SMWeaponVisibility)
	bool Visible = false;
};
