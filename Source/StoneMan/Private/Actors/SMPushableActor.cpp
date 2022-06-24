// Created by DChepurkin

#include "Actors/SMPushableActor.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogPushableActor, All, All);

ASMPushableActor::ASMPushableActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(StaticMesh);

	XAxisCollision = CreateDefaultSubobject<UBoxComponent>("XAxisCollision");
	XAxisCollision->SetupAttachment(GetRootComponent());
	XAxisCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	XAxisCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	XEndAxisCollision = CreateDefaultSubobject<UBoxComponent>("XEndAxisCollision");
	XEndAxisCollision->SetupAttachment(GetRootComponent());
	XEndAxisCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	XEndAxisCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	YAxisCollision = CreateDefaultSubobject<UBoxComponent>("YAxisCollision");
	YAxisCollision->SetupAttachment(GetRootComponent());
	YAxisCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	YAxisCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	YEndAxisCollision = CreateDefaultSubobject<UBoxComponent>("YEndAxisCollision");
	YEndAxisCollision->SetupAttachment(GetRootComponent());
	YEndAxisCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	YEndAxisCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ASMPushableActor::BeginPlay()
{
	Super::BeginPlay();

	XAxisCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnAxisCollisionBeginOverlap);
	YAxisCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnAxisCollisionBeginOverlap);

	XAxisCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnAxisCollisionEndOverlap);
	YAxisCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnAxisCollisionEndOverlap);

	XEndAxisCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndAxisCollisionEndOverlap);
	YEndAxisCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndAxisCollisionEndOverlap);
}

void ASMPushableActor::OnAxisCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                                   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	StaticMesh->IgnoreActorWhenMoving(OtherActor, true);
	IsPlayerCharacterOverlaped = IsActorHavePushComponent(OtherActor);
}

void ASMPushableActor::OnAxisCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                                 int32 OtherBodyIndex)
{
	IsPlayerCharacterOverlaped = !IsActorHavePushComponent(OtherActor);
}

void ASMPushableActor::OnEndAxisCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                                    int32 OtherBodyIndex)
{
	if(!OtherActor || !IsMoving) return;

	const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::KeepWorld, true);
	OtherActor->AttachToActor(this, AttachmentTransformRules);
}

void ASMPushableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const auto PushDuration = PushSpeed * DeltaTime * PushVector;
	AddActorWorldOffset(PushDuration, true);
}

const FTransform& ASMPushableActor::GetClosestPushTransform(const AActor* PushingActor)
{
	if(!PushingActor || PushTransforms.IsEmpty()) return FTransform::Identity;

	int32 ClosestIndex = 0;

	auto PushTransformWorldLocation = GetActorTransform().TransformPosition(PushTransforms[ClosestIndex].GetLocation());
	auto ClosestDistance = FVector::DistXY(PushingActor->GetActorLocation(), PushTransformWorldLocation);

	for(auto i = 1; i < PushTransforms.Num(); ++i)
	{
		PushTransformWorldLocation = GetActorTransform().TransformPosition(PushTransforms[i].GetLocation());
		const auto CurrentDistance = FVector::DistXY(PushingActor->GetActorLocation(), PushTransformWorldLocation);

		if(CurrentDistance < ClosestDistance)
		{
			ClosestDistance = CurrentDistance;
			ClosestIndex = i;
		}
	}

	return PushTransforms[ClosestIndex];
}

bool ASMPushableActor::CanMove()
{
	return IsPlayerCharacterOverlaped;
}

void ASMPushableActor::StartPush(const FVector& NewPushVector)
{
	PushVector = NewPushVector;
	IsMoving = true;
	SetActorTickEnabled(true);
}

void ASMPushableActor::StopPush()
{
	PushVector = FVector::ZeroVector;
	IsMoving = false;
	SetActorTickEnabled(false);
}

bool ASMPushableActor::IsActorHavePushComponent(AActor* Actor)
{
	return Actor ? Actor->FindComponentByClass<USMPushComponent>() != nullptr : false;
}
