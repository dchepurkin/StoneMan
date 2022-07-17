// Created by DChepurkin

#include "Character/Components/SMInteractionComponent.h"

#include "SMInteractionInterface.h"
#include "Components/BoxComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogInteractionComponent, All, All);

USMInteractionComponent::USMInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	InteractionCollision = CreateDefaultSubobject<UBoxComponent>("InteractionCollision");
	InteractionCollision->SetupAttachment(this);
	InteractionCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	InteractionCollision->SetWorldScale3D(FVector(1.5f, 1.25f, 2.f));
	InteractionCollision->SetRelativeLocation(FVector(75.f, 0.f, 0.f));
	InteractionCollision->SetBoxExtent(FVector(32.f));
}

void USMInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	check(InteractionCollision);

	InteractionCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	InteractionCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap);
}

void USMInteractionComponent::Interact()
{
	if(!CurrentInteractionActor || !GetOwner()) return;
	Cast<ISMInteractionInterface>(CurrentInteractionActor)->Interact(GetOwner());
}

void USMInteractionComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                             bool bFromSweep, const FHitResult& SweepResult)
{
	if(!OtherActor || !OtherActor->Implements<USMInteractionInterface>()) return;
	if(CurrentInteractionActor) Cast<ISMInteractionInterface>(CurrentInteractionActor)->SetOutlineVisible(false);
	CurrentInteractionActor = OtherActor;
	Cast<ISMInteractionInterface>(CurrentInteractionActor)->SetOutlineVisible(true);
}

void USMInteractionComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                           int32 OtherBodyIndex)
{
	if(!OtherActor || CurrentInteractionActor != OtherActor || !OtherActor->Implements<USMInteractionInterface>()) return;
	
	Cast<ISMInteractionInterface>(CurrentInteractionActor)->SetOutlineVisible(false);
	CurrentInteractionActor = nullptr;
}
