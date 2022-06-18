// Created by DChepurkin

#include "Character/PlayerCharacter/SMPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SMMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

ASMPlayerCharacter::ASMPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USMMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	bUseControllerRotationYaw = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetCapsuleComponent());
	SpringArmComponent->TargetArmLength = 600.f;
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ASMPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ThisClass::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ThisClass::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ThisClass::AddControllerPitchInput);
}

void ASMPlayerCharacter::MoveForward(const float AxisValue)
{
	if(FMath::IsNearlyZero(AxisValue)) return;
	
	const auto Rotation = GetControlRotation();
	const auto YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);

	const auto Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(Direction, AxisValue);
}

void ASMPlayerCharacter::MoveRight(const float AxisValue)
{
	if(FMath::IsNearlyZero(AxisValue)) return;

	const auto Rotation = GetControlRotation();
	const auto YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);

	const auto Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Direction, AxisValue);
}
