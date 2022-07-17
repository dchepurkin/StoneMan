// Created by DChepurkin

#include "Environments/Doors/SMClosedDoor.h"

DEFINE_LOG_CATEGORY_STATIC(LogClosedDoor, All, All);

#include "SMSwitchBase.h"

void ASMClosedDoor::BeginPlay()
{
	Super::BeginPlay();

	ClosePosition = DoorMesh->GetRelativeLocation();
	OpenPosition = FVector(ClosePosition.X, ClosePosition.Y, ClosePosition.Z + OpenZOffset);
	BindSwitchesesDelegates();

	OnChangeSwitch();
}

void ASMClosedDoor::BindSwitchesesDelegates()
{
	if(SwitchesesRules.IsEmpty()) return;

	for(auto& [Switch, OpenOn] : SwitchesesRules)
	{
		if(!Switch) continue;

		Switch->OnChangeSwitchEnabled.AddUObject(this, &ThisClass::OnChangeSwitch);
	}
}

void ASMClosedDoor::OnChangeSwitch()
{
	CheckForOpen() ? OpenDoor() : CloseDoor();
}

bool ASMClosedDoor::CheckForOpen()
{
	const auto CurrentSwitchRule = SwitchesesRules.FindByPredicate([](const FSwitches& SwitchRule)
	{
		if(!SwitchRule.Switch) return false;
		return SwitchRule.Switch->IsEnabled() ? SwitchRule.OpenOn != EOpenOn::OpenOnSwitchON : SwitchRule.OpenOn != EOpenOn::OpenOnSwitchOFF;
	});

	return CurrentSwitchRule ? false : true;
}

void ASMClosedDoor::OnOpenCloseDoor(const float Alpha)
{
	const auto CurrentPosition = FMath::Lerp(ClosePosition, OpenPosition, Alpha);
	DoorMesh->SetRelativeLocation(CurrentPosition);
}
