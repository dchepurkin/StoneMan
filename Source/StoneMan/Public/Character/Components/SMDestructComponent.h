// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SMDestructComponent.generated.h"

class UGeometryCollection;

USTRUCT(BlueprintType)
struct FDestructInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UGeometryCollection* DestructMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SocketName = NAME_None;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STONEMAN_API USMDestructComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USMDestructComponent();

	void Destruct();
	void SetMaterial(UMaterialInstance* NewMaterial) { ElementMaterial = NewMaterial; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=SMDestructComponent)
	TArray<FDestructInfo> DestructInfoArray = {
		{nullptr, "DestructHeadSocket"},
		{nullptr, "DestructUpperTorsSocket"},
		{nullptr, "DestructLowerTorsSocket"},
		{nullptr, "DestructLeftUpperArmSocket"},
		{nullptr, "DestructRightUpperArmSocket"},
		{nullptr, "DestructLeftLowerArmSocket"},
		{nullptr, "DestructRightLowerArmSocket"},
		{nullptr, "DestructLeftUpperLegSocket"},
		{nullptr, "DestructRightUpperLegSocket"},
		{nullptr, "DestructLeftLowerLegSocket"},
		{nullptr, "DestructRightLowerLegSocket"},
	};

private:
	UPROPERTY()
	UMaterialInstance* ElementMaterial = nullptr;
};
