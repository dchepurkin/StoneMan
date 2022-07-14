// Created by DChepurkin

#include "Actors/SMPushableActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"

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

	ElementComponent = CreateDefaultSubobject<USMElementComponent>("ElementComponent");
}

void ASMPushableActor::BeginPlay()
{
	Super::BeginPlay();

	check(ElementComponent);
	
	XAxisCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnAxisCollisionBeginOverlap);
	YAxisCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnAxisCollisionBeginOverlap);

	XAxisCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnAxisCollisionEndOverlap);
	YAxisCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnAxisCollisionEndOverlap);

	XEndAxisCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndAxisCollisionEndOverlap);
	YEndAxisCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndAxisCollisionEndOverlap);

	StartPosition = GetActorLocation();
}

void ASMPushableActor::OnAxisCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                                   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!GetPushComponent(OtherActor)) return;

	StaticMesh->ClearMoveIgnoreActors();
	StaticMesh->IgnoreActorWhenMoving(OtherActor, true);
	PushingActor = OtherActor;
}

void ASMPushableActor::OnAxisCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                                 int32 OtherBodyIndex)
{
	if(PushVector.IsZero() && GetPushComponent(OtherActor)) PushingActor = nullptr;
}

void ASMPushableActor::OnEndAxisCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                                    int32 OtherBodyIndex)
{
	if(!OtherActor || PushVector.IsZero()) return;

	const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::KeepWorld, true);
	OtherActor->AttachToActor(this, AttachmentTransformRules);
}

void ASMPushableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const auto PushDuration = PushSpeed * DeltaTime * PushVector;
	AddActorWorldOffset(PushDuration, true);

	if(!IsFreeBehindTheActor(PushVector))
	{
		if(const auto PushComponent = GetPushComponent(PushingActor)) PushComponent->RestartPush();
	}
}

void ASMPushableActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	StaticMesh->SetMaterial(0, ElementComponent->GetMaterial());
}

const FTransform& ASMPushableActor::GetClosestPushTransform(const AActor* Actor)
{
	if(!Actor || PushTransforms.IsEmpty()) return FTransform::Identity;

	int32 ClosestIndex = 0;

	auto PushTransformWorldLocation = GetActorTransform().TransformPosition(PushTransforms[ClosestIndex].GetLocation());
	auto ClosestDistance = FVector::DistXY(Actor->GetActorLocation(), PushTransformWorldLocation);

	for(auto i = 1; i < PushTransforms.Num(); ++i)
	{
		PushTransformWorldLocation = GetActorTransform().TransformPosition(PushTransforms[i].GetLocation());
		const auto CurrentDistance = FVector::DistXY(Actor->GetActorLocation(), PushTransformWorldLocation);

		if(CurrentDistance < ClosestDistance)
		{
			ClosestDistance = CurrentDistance;
			ClosestIndex = i;
		}
	}

	return PushTransforms[ClosestIndex];
}

bool ASMPushableActor::CanMove(const FVector& MoveDirection)
{
	return PushingActor != nullptr && IsFreeBehindTheActor(MoveDirection);
}

void ASMPushableActor::StartMoving(const FVector& NewPushVector)
{
	PushVector = NewPushVector;
	SetActorTickEnabled(true);
}

void ASMPushableActor::StopMoving()
{
	PushVector = FVector::ZeroVector;
	PushingActor = nullptr;
	SwitchAxisCollision();

	SetActorTickEnabled(false);
}

USMPushComponent* ASMPushableActor::GetPushComponent(const AActor* Actor)
{
	return Actor ? Actor->FindComponentByClass<USMPushComponent>() : nullptr;
}

bool ASMPushableActor::IsFreeBehindTheActor(const FVector& MoveDirection)
{
	if(!GetWorld() || !StaticMesh) return false;

	const auto TracePoint = GetActorLocation() + MoveDirection;

	FVector MinLocalBounds;
	FVector MaxLocalBounds;
	StaticMesh->GetLocalBounds(MinLocalBounds, MaxLocalBounds);

	const auto HalfSize = MaxLocalBounds * StaticMesh->GetComponentScale();
	FHitResult HitResult;

	UKismetSystemLibrary::BoxTraceSingle(GetWorld(),
	                                     TracePoint,
	                                     TracePoint,
	                                     HalfSize,
	                                     GetActorRotation(),
	                                     UEngineTypes::ConvertToTraceType(ECC_Visibility),
	                                     false,
	                                     {this},
	                                     EDrawDebugTrace::None,
	                                     HitResult,
	                                     true);

	return !HitResult.bBlockingHit;
}

void ASMPushableActor::SwitchAxisCollision() const
{
	XAxisCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	YAxisCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	XAxisCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	YAxisCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
