// Created by DChepurkin

#include "SMPlayerCharacter.h"
#include "SMHealthComponent.h"
#include "SMPushComponent.h"
#include "SMCameraComponent.h"
#include "SMElementComponent.h"
#include "SMWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SMMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerCharacter, All, All);

ASMPlayerCharacter::ASMPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USMMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.TickInterval = 0.1f;
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
	SpringArmComponent->CameraLagMaxDistance = 20.f;

	CameraComponent = CreateDefaultSubobject<USMCameraComponent>("CameraComponent");
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
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ThisClass::Attack);
}

void ASMPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(PushComponent)
	check(SpringArmComponent)
	check(CameraComponent)

	PushComponent->OnStartPush.AddUObject(this, &ThisClass::OnStartPush);
	PushComponent->OnStopPush.AddUObject(this, &ThisClass::OnStopPush);

	CameraComponent->OnCameraBeginOverlap.AddUObject(this, &ThisClass::OnCameraBeginOverlap);
	CameraComponent->OnCameraEndOverlap.AddUObject(this, &ThisClass::OnCameraEndOverlap);
}

void ASMPlayerCharacter::MoveForward(float AxisValue)
{
	if(PlayerState == ESMPlayerState::Push || FMath::IsNearlyZero(AxisValue)) return;

	const auto Rotation = GetControlRotation();
	const auto YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);
	const auto Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(Direction, AxisValue);
}

void ASMPlayerCharacter::MoveRight(const float AxisValue)
{
	if(PlayerState == ESMPlayerState::Push || FMath::IsNearlyZero(AxisValue)) return;

	const auto Rotation = GetControlRotation();
	const auto YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);
	const auto Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Direction, AxisValue);
}

void ASMPlayerCharacter::Jump()
{
	if(PlayerState == ESMPlayerState::Push) PushComponent->RestartPush();
	if(HealthComponent->IsFallingVelocityIsDamaged(GetVelocity())) return;
	Super::Jump();
}

void ASMPlayerCharacter::OnStartPush()
{
	SetState(ESMPlayerState::Push);
}

void ASMPlayerCharacter::OnStopPush()
{
	SetState(ESMPlayerState::Idle);
}

void ASMPlayerCharacter::OnDeath()
{
	Super::OnDeath();
	const auto PlayerContoller = GetController<APlayerController>();
	if(PlayerContoller) DisableInput(PlayerContoller);

	PushComponent->StopPush();
}

void ASMPlayerCharacter::OnCameraBeginOverlap(AActor* Actor)
{
	SetMeshVisibility(Actor != this);
}

void ASMPlayerCharacter::OnCameraEndOverlap()
{
	SetMeshVisibility(true);
}

void ASMPlayerCharacter::SetMeshVisibility(const bool Enabled)
{
	if(!GetMesh()) return;
	GetMesh()->SetVisibility(Enabled, true);
}

void ASMPlayerCharacter::Attack()
{
	if(PlayerState == ESMPlayerState::Idle &&
		!HealthComponent->IsDead() &&
		!GetCharacterMovement()->IsFalling())
	{
		WeaponComponent->StartAttack(ElementComponent->GetElement());
	}
}
