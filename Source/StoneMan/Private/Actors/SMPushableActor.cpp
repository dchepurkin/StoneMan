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
	StaticMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	SetRootComponent(StaticMesh);

	CreateCollision(XAxisCollision, "XAxisCollision");
	CreateCollision(XEndAxisCollision, "XEndAxisCollision");
	CreateCollision(YAxisCollision, "YAxisCollision");
	CreateCollision(YEndAxisCollision, "YEndAxisCollision");

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

	if(!IsClearToMove(PushVector))
	{
		if(const auto PushComponent = GetPushComponent(PushingActor)) PushComponent->RestartPush();
	}
}

void ASMPushableActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UpdateColor();
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
	return PushingActor != nullptr && IsClearToMove(MoveDirection);
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

void ASMPushableActor::UpdateColor()
{
	StaticMesh->SetVectorParameterValueOnMaterials(ElementComponent->GetMaterialParameterName(), FVector(ElementComponent->GetElementColor()));
}

USMPushComponent* ASMPushableActor::GetPushComponent(const AActor* Actor)
{
	return Actor ? Actor->FindComponentByClass<USMPushComponent>() : nullptr;
}

bool ASMPushableActor::IsClearToMove(const FVector& MoveDirection)
{
	if(!StaticMesh) return false;
	
	const auto TracePoint = GetActorLocation() + MoveDirection;
	FVector MinLocalBounds;
	FVector MaxLocalBounds;
	StaticMesh->GetLocalBounds(MinLocalBounds, MaxLocalBounds);
	const auto HalfSize = MaxLocalBounds * StaticMesh->GetComponentScale();

	return CheckWals(TracePoint, HalfSize) && CheckFloor(TracePoint, HalfSize);
}

bool ASMPushableActor::CheckWals(const FVector& TracePoint, const FVector& HalfSize)
{
	if(!GetWorld()) return false;

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

bool ASMPushableActor::CheckFloor(const FVector& TracePoint, const FVector& HalfSize) const
{
	if(!GetWorld()) return false;

	const auto TraceEnd = TracePoint - FVector(0.f, 0.f, 1.f) * HalfSize - FVector(0.f, .0f, 10.f);
	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(HitResult, TracePoint, TraceEnd, ECC_WorldStatic, CollisionQueryParams);

	return HitResult.bBlockingHit;
}

void ASMPushableActor::SwitchAxisCollision() const
{
	XAxisCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	YAxisCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	XAxisCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	YAxisCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ASMPushableActor::CreateCollision(UBoxComponent*& Collision, const FName& CollisionName)
{
	Collision = CreateDefaultSubobject<UBoxComponent>(CollisionName);
	Collision->SetupAttachment(GetRootComponent());
	Collision->SetCollisionResponseToAllChannels(ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}
