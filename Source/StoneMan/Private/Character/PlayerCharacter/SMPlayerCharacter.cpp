// Created by DChepurkin

#include "Character/PlayerCharacter/SMPlayerCharacter.h"

#include "SMPushComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SMMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerCharacter, All, All);

ASMPlayerCharacter::ASMPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USMMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	JumpMaxCount = 2;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetCapsuleComponent());
	SpringArmComponent->TargetArmLength = 600.f;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->bEnableCameraRotationLag = true;
	SpringArmComponent->CameraLagSpeed = 8.f;
	SpringArmComponent->CameraRotationLagSpeed = 8.f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	PushComponent = CreateDefaultSubobject<USMPushComponent>("PushComponent");
}

void ASMPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ThisClass::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ThisClass::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ThisClass::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ThisClass::Jump);
	PlayerInputComponent->BindAction("Push", IE_Pressed, PushComponent, &USMPushComponent::StartPush);
	PlayerInputComponent->BindAction("Push", IE_Released, PushComponent, &USMPushComponent::StopPush);
}

void ASMPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASMPlayerCharacter::MoveForward(float AxisValue)
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

void ASMPlayerCharacter::Jump()
{
	Super::Jump();
}
