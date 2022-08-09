// Created by DChepurkin

#include "SMPlayerCharacter.h"
#include "SMHealthComponent.h"
#include "SMPushComponent.h"
#include "SMCameraComponent.h"
#include "SMElementComponent.h"
#include "SMInteractionComponent.h"
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
	SpringArmComponent->SocketOffset = FVector(0.f, 0.f, 100.f);

	CameraComponent = CreateDefaultSubobject<USMCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	PushComponent = CreateDefaultSubobject<USMPushComponent>("PushComponent");

	InteractionComponent = CreateDefaultSubobject<USMInteractionComponent>("InteractionComponent");
	InteractionComponent->SetupAttachment(GetRootComponent());
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
	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &ThisClass::OnInteraction);

	DECLARE_DELEGATE_OneParam(FOnSprintSignature, const bool);
	PlayerInputComponent->BindAction<FOnSprintSignature>("Sprint", IE_Pressed, this, &ThisClass::SetSprintEnabled, true);
	PlayerInputComponent->BindAction<FOnSprintSignature>("Sprint", IE_Released, this, &ThisClass::SetSprintEnabled, false);

	DECLARE_DELEGATE_OneParam(FOnTryToSetElementSignature, const ESMElement);
	PlayerInputComponent->BindAction<FOnTryToSetElementSignature>("ElementIce", IE_Pressed, this, &ASMPlayerCharacter::OnTryToSetElement,
	                                                              ESMElement::Ice);
	PlayerInputComponent->BindAction<FOnTryToSetElementSignature>("ElementFire", IE_Pressed, this, &ASMPlayerCharacter::OnTryToSetElement,
	                                                              ESMElement::Fire);
	PlayerInputComponent->BindAction<FOnTryToSetElementSignature>("ElementAir", IE_Pressed, this, &ASMPlayerCharacter::OnTryToSetElement,
	                                                              ESMElement::Air);
	PlayerInputComponent->BindAction<FOnTryToSetElementSignature>("ElementEarth", IE_Pressed, this, &ASMPlayerCharacter::OnTryToSetElement,
	                                                              ESMElement::Earth);
}

void ASMPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(PushComponent);
	check(SpringArmComponent);
	check(CameraComponent);
	check(InteractionComponent);

	PushComponent->OnStartPush.AddUObject(this, &ThisClass::OnStartPush);
	PushComponent->OnStopPush.AddUObject(this, &ThisClass::OnStopPush);

	CameraComponent->OnCameraBeginOverlap.AddUObject(this, &ThisClass::OnCameraBeginOverlap);
	CameraComponent->OnCameraEndOverlap.AddUObject(this, &ThisClass::OnCameraEndOverlap);

	WeaponComponent->OnStartNextComboSection.AddUObject(this, &ThisClass::OnStartNextComboSection);

	ElementComponent->OnChangeElement.AddUObject(this, &ThisClass::OnChangeElement);
	ChangeColor();
}

void ASMPlayerCharacter::MoveForward(float AxisValue)
{
	if(FMath::IsNearlyZero(AxisValue))
	{
		if(PlayerState == ESMPlayerState::Push) PushComponent->StopPush();
		return;
	}

	if(PlayerState == ESMPlayerState::Push) return;

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
	if(HealthComponent->IsFallingVelocityIsDamaged(GetVelocity()) || PlayerState == ESMPlayerState::Attack) return;
	Super::Jump();
}

void ASMPlayerCharacter::SetSprintEnabled(const bool Enabled)
{
	const auto MovementComponent = Cast<USMMovementComponent>(GetCharacterMovement());
	if(!MovementComponent) return;

	MovementComponent->SetSprintEnabled(Enabled && PlayerState == ESMPlayerState::Idle);
}

void ASMPlayerCharacter::OnStartPush()
{
	SetState(ESMPlayerState::Push);
	SetSprintEnabled(false);
}

void ASMPlayerCharacter::OnStopPush()
{
	if(PlayerState == ESMPlayerState::Push) SetState(ESMPlayerState::Idle);
}

void ASMPlayerCharacter::OnDeath()
{
	Super::OnDeath();
	const auto PlayerContoller = GetController<APlayerController>();
	if(PlayerContoller) DisableInput(PlayerContoller);

	PushComponent->StopPush();
	Cast<USMMovementComponent>(GetCharacterMovement())->StopStaminaChange();
}

bool ASMPlayerCharacter::CanAttack() const
{
	return (PlayerState == ESMPlayerState::Idle || PlayerState == ESMPlayerState::Attack)
		&& !GetCharacterMovement()->IsFalling()
		&& Super::CanAttack();
}

void ASMPlayerCharacter::OnStartAttack()
{
	if(!AttackAnimMontage) return;
	PlayAnimMontage(AttackAnimMontage);
	SetState(ESMPlayerState::Attack);
	SetSprintEnabled(false);
}

void ASMPlayerCharacter::OnEndAttack()
{
	SetState(ESMPlayerState::Idle);
}

void ASMPlayerCharacter::OnStartNextComboSection(const FName& NextComboSectionName)
{
	if(!AttackAnimMontage) return;
	SetNextComboSection(NextComboSectionName);
}

void ASMPlayerCharacter::SetNextComboSection(const FName& NextComboSectionName) const
{
	if(!GetMesh()) return;

	const auto AnimInstance = GetMesh()->GetAnimInstance();
	if(!AnimInstance || !AnimInstance->Montage_IsPlaying(AttackAnimMontage)) return;

	const auto CurrentComboSectionName = AnimInstance->Montage_GetCurrentSection();
	AnimInstance->Montage_SetNextSection(CurrentComboSectionName, NextComboSectionName, AttackAnimMontage);
	WeaponComponent->SetNextComboSectionEnabled(false);
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

void ASMPlayerCharacter::OnTryToSetElement(const ESMElement NewElement)
{
	if(NewElement == ElementComponent->GetElement() || PlayerState != ESMPlayerState::Idle || GetCharacterMovement()->IsFalling()) return;
	SetState(ESMPlayerState::ChangeElement);
	ElementComponent->SetElement(NewElement);
}

void ASMPlayerCharacter::OnChangeElement()
{
	if(!ChangeElementAnimMontage) return;
	PlayAnimMontage(ChangeElementAnimMontage);
}

void ASMPlayerCharacter::OnInteraction()
{
	if(PlayerState != ESMPlayerState::Idle || GetCharacterMovement()->IsFalling()) return;
	InteractionComponent->Interact();
}

void ASMPlayerCharacter::ChangeColor() const
{
	if(!GetMesh()) return;

	GetMesh()->SetVectorParameterValueOnMaterials(ElementComponent->GetMaterialParameterName(), FVector(ElementComponent->GetElementColor()));
}
