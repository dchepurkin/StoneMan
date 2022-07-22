// Created by DChepurkin

#include "Actors/SMLaserCube.h"

#include "SMLaserComponent.h"
#include "Components/BoxComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogLaserCube, All, All);

ASMLaserCube::ASMLaserCube()
{
	LaserComponent = CreateDefaultSubobject<USMLaserComponent>("LaserComponent");
	LaserComponent->SetupAttachment(GetRootComponent());
	StaticMesh->CustomDepthStencilValue = 1;

	CreateLaserTrigger(XLaserTrigger, "XLaserTrigger");
	CreateLaserTrigger(YLaserTrigger, "YLaserTrigger");
}

void ASMLaserCube::CreateLaserTrigger(UBoxComponent*& LaserTrigger, const FName& TriggerName)
{
	LaserTrigger = CreateDefaultSubobject<UBoxComponent>(TriggerName);
	LaserTrigger->SetupAttachment(GetRootComponent());
	LaserTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	LaserTrigger->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	LaserTrigger->ComponentTags.Add("LaserTrigger");
}

void ASMLaserCube::BeginPlay()
{
	Super::BeginPlay();

	UpdateLaserColor();

	LaserComponent->OnDetectLaserTrigger.AddUObject(this, &ThisClass::OnDetectLaserTrigger);
	LaserComponent->OnLoseLaserTrigger.AddUObject(this, &ThisClass::OnLoseLaserTrigger);
}

void ASMLaserCube::UpdateLaserColor() const
{
	LaserComponent->SetLaserColor(ElementComponent->GetElementColor());
}

void ASMLaserCube::OnDetectLaserTrigger(AActor* Actor)
{
	if(!Actor || !Actor->Implements<USMLaserInterface>()) return;

	Cast<ISMLaserInterface>(Actor)->Activate(ElementComponent->GetElement());
}

void ASMLaserCube::OnLoseLaserTrigger(AActor* Actor)
{
	if(!Actor || !Actor->Implements<USMLaserInterface>()) return;

	Cast<ISMLaserInterface>(Actor)->Disactivate();
}

void ASMLaserCube::Interact(AActor* WhoInteract)
{
	if(LaserComponent->IsEnabled()) LaserComponent->RotateLaser();
}

void ASMLaserCube::SetOutlineVisible(const bool Visible)
{
	StaticMesh->SetRenderCustomDepth(Visible && LaserComponent->IsEnabled());
}

void ASMLaserCube::UpdateColor()
{
	if(!LaserComponent->IsEnabled()) ElementComponent->SetElement(ESMElement::NoElement);
	Super::UpdateColor();
}

void ASMLaserCube::Activate(const ESMElement Element)
{
	if(!ElementComponent || !LaserComponent || LaserComponent->IsEnabled()) return;

	ElementComponent->SetElement(Element);
	LaserComponent->SetLaserEnabled(true);
	UpdateColor();
	UpdateLaserColor();
}

void ASMLaserCube::Disactivate()
{
	if(!ElementComponent || !LaserComponent || LaserComponent->IsUndisablable()) return;

	ElementComponent->SetElement(ESMElement::NoElement);
	UpdateColor();
	LaserComponent->SetLaserEnabled(false);
}

void ASMLaserCube::ResetCondition()
{
	Super::ResetCondition();
	if(LaserComponent) LaserComponent->ResetDirection();
}
