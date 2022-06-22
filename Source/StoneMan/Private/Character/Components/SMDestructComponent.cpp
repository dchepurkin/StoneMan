// Created by DChepurkin

#include "Character/Components/SMDestructComponent.h"

#include "GameFramework/Character.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogDestructComponent, All, All);

USMDestructComponent::USMDestructComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USMDestructComponent::Destruct()
{
	const auto Character = GetOwner<ACharacter>();
	if(!Character) return;

	const auto Mesh = Character->GetMesh();
	if(!Mesh) return;

	for(const auto [DestructMesh, SocketName] : DestructInfoArray)
	{
		if(!DestructMesh) return;

		const auto GeometryComponent = NewObject<UGeometryCollectionComponent>(Character, UGeometryCollectionComponent::StaticClass());
		if(!GeometryComponent) continue;

		GeometryComponent->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
		GeometryComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		GeometryComponent->RestCollection = DestructMesh;
		GeometryComponent->EnableClustering = false;		
		GeometryComponent->RegisterComponent();
	}

	Mesh->DestroyComponent();
}
