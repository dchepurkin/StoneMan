// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SMCharacterBase.generated.h"

class UGeometryCollectionComponent;
class USMDestructComponent;

UCLASS(Abstract)
class STONEMAN_API ASMCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	explicit ASMCharacterBase(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	USMDestructComponent* DestructComponent;

private:
};
