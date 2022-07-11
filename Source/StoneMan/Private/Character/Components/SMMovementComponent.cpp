// Created by DChepurkin

#include "Character/Components/SMMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogSMMovement, All, All);

USMMovementComponent::USMMovementComponent()
{
	bUseControllerDesiredRotation = true;
	bOrientRotationToMovement = true;
	RotationRate = FRotator(0.f, 400.f, 0.f);
	JumpZVelocity = 300.f;
	MaxAcceleration = 1500.f;
}

void USMMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	SetStamina(MaxStamina);
}

float USMMovementComponent::GetMaxSpeed() const
{
	return bSprint ? SprintMaxSpeed : bWalk ? WalkMaxSpeed : CanRun() ? Super::GetMaxSpeed() : WalkMaxSpeed;
}

void USMMovementComponent::SetSprintEnabled(const bool Enabled)
{
	bSprint = Enabled && CanSprint();
	bWalk = false;

	SetStaminaUseTimerEnabled(bSprint);
	SetStaminaRestoreTimerEnabled(!bSprint);
}

void USMMovementComponent::SwitchWalk()
{
	bWalk = !bWalk;
}

void USMMovementComponent::StopStaminaChange()
{
	SetStaminaRestoreTimerEnabled(false);
	SetStaminaUseTimerEnabled(false);
}

void USMMovementComponent::SetStamina(const float NewStamina)
{
	CurrentStamina = FMath::Clamp<float>(NewStamina, 0.f, MaxStamina);
	OnStaminaChanged.Broadcast(CurrentStamina);
}

void USMMovementComponent::OnRestoreStamina()
{
	SetStamina(CurrentStamina + StaminaRestoreAmount);
	if(FMath::IsNearlyEqual(CurrentStamina, MaxStamina)) SetStaminaRestoreTimerEnabled(false);
}

void USMMovementComponent::OnUseStamina()
{
	SetStamina(CurrentStamina - SprintCoast);
	if(FMath::IsNearlyZero(CurrentStamina)) SetSprintEnabled(false);
}

void USMMovementComponent::SetStaminaUseTimerEnabled(const float Enabled)
{
	if(!GetWorld()) return;

	Enabled
		? GetWorld()->GetTimerManager().SetTimer(StaminaUseTimerHandle, this, &ThisClass::OnUseStamina, StaminaUseRate, true)
		: GetWorld()->GetTimerManager().ClearTimer(StaminaUseTimerHandle);
}

void USMMovementComponent::SetStaminaRestoreTimerEnabled(const float Enabled)
{
	if(!GetWorld()) return;

	Enabled
		? GetWorld()->GetTimerManager().SetTimer(StaminaRestoreTimerHandle, this, &ThisClass::OnRestoreStamina, StaminaRestoreRate, true, StaminaRestoreDelay)
		: GetWorld()->GetTimerManager().ClearTimer(StaminaRestoreTimerHandle);
}
