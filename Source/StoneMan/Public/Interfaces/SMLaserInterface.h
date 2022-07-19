// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "SMCoreTypes.h"
#include "UObject/Interface.h"
#include "SMLaserInterface.generated.h"

UINTERFACE(MinimalAPI)
class USMLaserInterface : public UInterface
{
	GENERATED_BODY()
};

class STONEMAN_API ISMLaserInterface
{
	GENERATED_BODY()
	
public:
	virtual void Activate(const ESMElement Element) = 0;
	virtual void Disactivate() = 0;
};
