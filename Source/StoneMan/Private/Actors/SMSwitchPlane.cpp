// Created by DChepurkin

#include "Actors/SMSwitchPlane.h"

#include "Components/BoxComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogSwitchPlane, All, All);

ASMSwitchPlane::ASMSwitchPlane()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 0.1f;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
	StaticMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	StaticMesh->SetupAttachment(GetRootComponent());

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	BoxCollision->SetupAttachment(GetRootComponent());
}

void ASMSwitchPlane::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(SwitchCheck())
	{
		SetSwitchEnabled(true);
		SetActorTickEnabled(false);
	}
}

void ASMSwitchPlane::BeginPlay()
{
	Super::BeginPlay();

	check(StaticMesh);
	check(BoxCollision);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap);
}

void ASMSwitchPlane::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                    bool bFromSweep, const FHitResult& SweepResult)
{
	if(!bSwitched) SetActorTickEnabled(true);
}

void ASMSwitchPlane::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TArray<AActor*> OverlapedActors;
	BoxCollision->GetOverlappingActors(OverlapedActors);

	if(!OverlapedActors.IsEmpty()) return;

	if(bSwitched) SetSwitchEnabled(false);
	SetActorTickEnabled(false);
}

bool ASMSwitchPlane::SwitchCheck() const
{
	FVector MinBounds;
	FVector MaxBounds;
	StaticMesh->GetLocalBounds(MinBounds, MaxBounds);

	const auto ScaledMinBounds = MinBounds * StaticMesh->GetComponentScale();
	const auto ScaledMaxBounds = MaxBounds * StaticMesh->GetComponentScale();

	const auto MinActorLocation = ScaledMinBounds + GetActorLocation();
	const auto MaxActorLocation = ScaledMaxBounds + GetActorLocation();

	return LineTrace(MinActorLocation.X, MinActorLocation.Y, MaxActorLocation.X, MinActorLocation.Y)
		&& LineTrace(MinActorLocation.X, MaxActorLocation.Y, MaxActorLocation.X, MaxActorLocation.Y)
		&& LineTrace(MinActorLocation.X, MinActorLocation.Y, MinActorLocation.X, MaxActorLocation.Y)
		&& LineTrace(MaxActorLocation.X, MinActorLocation.Y, MaxActorLocation.X, MaxActorLocation.Y);
}

bool ASMSwitchPlane::LineTrace(const float X1, const float Y1, const float X2, const float Y2) const
{
	if(!GetWorld()) return false;

	const auto Z = GetActorLocation().Z + 20.f;
	FHitResult HitResult;

	const auto Start = FVector(X1, Y1, Z);
	const auto End = FVector(X2, Y2, Z);

	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	if(bDrawDebugLines) DrawDebugLine(GetWorld(), Start, End, DebugColor, false, PrimaryActorTick.TickInterval, 0, 1.f);

	return !HitResult.bBlockingHit;
}

void ASMSwitchPlane::SetSwitchEnabled(const bool Enabled)
{
	Super::SetSwitchEnabled(Enabled);
	
	const auto Offset = FVector(0.f, 0.f, SwitchedZOffset);
	StaticMesh->AddLocalOffset(Enabled ? -Offset : Offset);	
}
