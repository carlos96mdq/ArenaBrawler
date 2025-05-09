// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/PlayerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(LogPlayer);

APlayerCharacter::APlayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// This can be tweaked in the Character Blueprint in Editor
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera spring arm (pulls in towards the player if there is a collision)
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraSpringArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName); // Attach the camera to the end of the spring arm and let the arm adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) are set in the derived blueprint asset
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Bind the AnimMontage notification with callbacks
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &APlayerCharacter::OnMontageNotifyBegin);
	}
}

void APlayerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Attacks
		EnhancedInputComponent->BindAction(BasicAttackAction, ETriggerEvent::Completed, this, &APlayerCharacter::BasicAttack);
		
		// Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayerCharacter::StartSprinting);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprinting);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
	}
	else
	{
		UE_LOG(LogPlayer, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APlayerCharacter::StartSprinting(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void APlayerCharacter::StopSprinting(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (CurrentAttack != 0)
	{
		return;
	}

	// Input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward vector and right vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	// Input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::BasicAttack(const FInputActionValue& Value)
{
	AttackStarts(1);
}

void APlayerCharacter::AttackStarts(int Attack)
{
	if (CurrentAttack != 0)
	{
		return;
	}

	switch (Attack)
	{
	case 1:	// Basic attack
		UE_LOG(LogPlayer, Display, TEXT("'%s' used base attack"), *GetNameSafe(this));

		if (BasicAttackMontage && GetMesh()->GetAnimInstance())
		{
			CurrentAttack = Attack;
			GetMesh()->GetAnimInstance()->Montage_Play(BasicAttackMontage);
		}
		break;
	default:
		break;
	}
}

void APlayerCharacter::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPayload)
{
	if (NotifyName == "DealDamage")
	{
		AttackDealsDamage();
	}
	else if (NotifyName == "AttackEnded")
	{
		AttackEnded();
	}
}

void APlayerCharacter::AttackDealsDamage()
{
	//TODO For now we don't care about the attack type. Just do damage.
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * BasicAttackRange;
	FCollisionShape Shape = FCollisionShape::MakeSphere(1.0f);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_Pawn, Shape, Params);
	//DrawDebugCylinder(GetWorld(), Start, End, 1.0f, 5, FColor::Red, true, 10.0f);

	if (bHit)
	{
		ABaseCharacter* HitCharacter = Cast<ABaseCharacter>(HitResult.GetActor());
		if (HitCharacter)
		{
			HitCharacter->ReceiveDamage(20);
		}
	}
}

void APlayerCharacter::AttackEnded()
{
	CurrentAttack = 0;
}