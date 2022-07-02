// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SMNextComboSectionNotify.generated.h"

class USMWeaponComponent;

UCLASS()
class STONEMAN_API USMNextComboSectionNotify : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override { return FString("ComboSection"); }

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category=SMNextComboSection)
	FName NextComboSectionName = "2";

private:
	USMWeaponComponent* GetWeaponComponent(const USkeletalMeshComponent* MeshComp) const;
};
