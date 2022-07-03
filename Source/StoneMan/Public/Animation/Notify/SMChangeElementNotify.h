// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SMChangeElementNotify.generated.h"

class ASMPlayerCharacter;

UCLASS()
class STONEMAN_API USMChangeElementNotify : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override { return FString("ChangeElement"); }
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	ASMPlayerCharacter* GetPlayerCharacter(USkeletalMeshComponent* MeshComp);
};
