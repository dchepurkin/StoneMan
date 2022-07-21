// Created by DChepurkin

#include "Actors/SMLaserSwitch.h"

#include "SMElementComponent.h"
#include "Components/BoxComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogLaserSwitch, All, All);

ASMLaserSwitch::ASMLaserSwitch()
{
	ElementComponent = CreateDefaultSubobject<USMElementComponent>("ElementComponent");

	StaticMech = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(StaticMech);

	LaserTrigger = CreateDefaultSubobject<UBoxComponent>("LaserTrigger");
	LaserTrigger->SetupAttachment(GetRootComponent());
	LaserTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	LaserTrigger->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	LaserTrigger->ComponentTags.Add("LaserTrigger");
}

void ASMLaserSwitch::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetElement();
}

void ASMLaserSwitch::BeginPlay()
{
	Super::BeginPlay();
	UpdateEmissive();
}

void ASMLaserSwitch::SetSwitchEnabled(const bool Enabled)
{
	Super::SetSwitchEnabled(Enabled);
	UpdateEmissive();
}

void ASMLaserSwitch::Activate(const ESMElement Element)
{
	if(Element != ElementComponent->GetElement()) return;

	SetSwitchEnabled(true);
}

void ASMLaserSwitch::Disactivate()
{
	SetSwitchEnabled(false);
}

void ASMLaserSwitch::UpdateEmissive() const
{
	StaticMech->SetScalarParameterValueOnMaterials(EmissiveParamName, IsEnabled() ? EmissiveON : EmissiveOFF);
}

void ASMLaserSwitch::SetElement() const
{
	StaticMech->SetMaterial(ElementComponent->GetMaterialElementIndex(), ElementComponent->GetElementMaterial());
	StaticMech->SetVectorParameterValueOnMaterials(ElementComponent->GetMaterialParameterName(), FVector(ElementComponent->GetElementColor()));
}
