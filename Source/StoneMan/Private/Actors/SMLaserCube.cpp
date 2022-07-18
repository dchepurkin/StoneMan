// Created by DChepurkin

#include "Actors/SMLaserCube.h"

#include "SMLaserComponent.h"

ASMLaserCube::ASMLaserCube()
{
	LaserComponent = CreateDefaultSubobject<USMLaserComponent>("LaserComponent");
}

void ASMLaserCube::BeginPlay()
{
	Super::BeginPlay();

	UpdateLaserColor();
}

void ASMLaserCube::UpdateLaserColor() const
{
	LaserComponent->SetLaserColor(ElementComponent->GetElementColor());
}
