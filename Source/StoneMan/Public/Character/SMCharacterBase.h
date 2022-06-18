// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SMCharacterBase.generated.h"

UCLASS()
class STONEMAN_API ASMCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	explicit ASMCharacterBase(const FObjectInitializer& ObjectInitializer);	

protected:
	virtual void BeginPlay() override;
};
