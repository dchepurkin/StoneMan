// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SMInteractionInterface.generated.h"

UINTERFACE(MinimalAPI)
class USMInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class STONEMAN_API ISMInteractionInterface
{
	GENERATED_BODY()

public:
	virtual void Interact(AActor* WhoInteract) = 0;
	virtual FString GetInteractionMessage() const = 0;
};
