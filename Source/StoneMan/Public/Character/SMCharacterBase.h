// Created by DChepurkin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SMCharacterBase.generated.h"

class UGeometryCollectionComponent;

UCLASS(Abstract)
class STONEMAN_API ASMCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	explicit ASMCharacterBase(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UGeometryCollectionComponent* Head;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UGeometryCollectionComponent* Tors;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UGeometryCollectionComponent* LeftShoulder;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UGeometryCollectionComponent* RightShoulder;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UGeometryCollectionComponent* LeftHand;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UGeometryCollectionComponent* RightHand;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UGeometryCollectionComponent* Belt;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UGeometryCollectionComponent* LeftLeg;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	UGeometryCollectionComponent* RightLeg;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="SMCharacter|DestructableBodyPartsSockets")
	FName HeadSocketName = "DestructHeadSocket";

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="SMCharacter|DestructableBodyPartsSockets")
	FName TorsSocketName = "DestructTorsSocket";

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="SMCharacter|DestructableBodyPartsSockets")
	FName LeftShoulderSocketName = "DestructLeftShoulderSocket";

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="SMCharacter|DestructableBodyPartsSockets")
	FName RightShoulderSocketName = "DestructRightShoulderSocket";

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="SMCharacter|DestructableBodyPartsSockets")
	FName LeftHandSocketName = "DestructLeftHandSocket";

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="SMCharacter|DestructableBodyPartsSockets")
	FName RightHandSocketName = "DestructRightHandSocket";

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="SMCharacter|DestructableBodyPartsSockets")
	FName BeltSocketName = "DestructBeltSocket";

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="SMCharacter|DestructableBodyPartsSockets")
	FName LeftLegSocketName = "DestructLeftLegSocket";

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="SMCharacter|DestructableBodyPartsSockets")
	FName RightLegSocketName = "DestructRightLegSocket";

private:
	void CreateGeometryCollectionComponent(UGeometryCollectionComponent*& GeometryCollection, const FName& Name);
};
