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
}

void ASMClosedDoor::BindSwitchesesDelegates()
{
	if(SwitchesesRules.IsEmpty()) return;

	for(auto& [Switch, OpenOn, SwitchIsOpen] : SwitchesesRules)
	{
		if(!Switch) continue;

		Switch->OnChangeSwitchEnabled.AddUObject(this, &ThisClass::OnChangeSwitchEnabled);
		SwitchIsOpen = OpenOn != EOpenOn::OpenOnSwitchON;
	}
}

void ASMClosedDoor::OnChangeSwitchEnabled(AActor* Switch, const bool Enabled)
{
	const auto CurrentSwitchRule = SwitchesesRules.FindByPredicate([&](const FSwitches& SwitchRule)
	{
		return SwitchRule.Switch == Switch;
	});

	if(!CurrentSwitchRule) return;

	CurrentSwitchRule->SwitchIsOpen = CurrentSwitchRule->OpenOn == EOpenOn::OpenOnSwitchON ? Enabled : !Enabled;

	CheckForOpen()
		? OpenDoor()
		: CloseDoor();
}

bool ASMClosedDoor::CheckForOpen()
{
	const auto CurrentSwitchRule = SwitchesesRules.FindByPredicate([](const FSwitches& SwitchRule)
	{
		return !SwitchRule.SwitchIsOpen;
	});

	return CurrentSwitchRule ? false : true;
}

void ASMClosedDoor::OnOpenCloseDoor(const float Alpha)
{
	const auto CurrentPosition = FMath::Lerp(ClosePosition, OpenPosition, Alpha);
	DoorMesh->SetRelativeLocation(CurrentPosition);
}
