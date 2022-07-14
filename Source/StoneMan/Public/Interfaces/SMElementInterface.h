// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SMCoreTypes.h"
#include "SMElementInterface.generated.h"

UINTERFACE(MinimalAPI)
class USMElementInterface : public UInterface
{
	GENERATED_BODY()
};

class STONEMAN_API ISMElementInterface
{
	GENERATED_BODY()

public:
	virtual ESMElement GetElement() = 0;
};
