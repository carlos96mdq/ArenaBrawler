// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"

DEFINE_LOG_CATEGORY(LogCharacter);

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind the AnimMontage notification with callbacks
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &ABaseCharacter::OnMontageNotifyBegin);
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::AttackStarts(int Attack)
{
	if (CurrentAttack != 0)
	{
		return;
	}

	switch (Attack)
	{
	case 1:	// Basic attack
		UE_LOG(LogCharacter, Display, TEXT("'%s' used basic attack"), *GetNameSafe(this));

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

void ABaseCharacter::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPayload)
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

void ABaseCharacter::AttackDealsDamage()
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

void ABaseCharacter::AttackEnded()
{
	CurrentAttack = 0;
}

void ABaseCharacter::ReceiveDamage(float Amount)
{
	Health -= Amount;

	if (GEngine)
	{
		FString DebugMsg = FString::Printf(TEXT("%s lost life and now has a current health of: %.2f"), *GetNameSafe(this),Health);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
	}

	if (ReceiveDamageMontage && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(ReceiveDamageMontage);
	}

	if (Health <= 0)
	{
		Die();
	}
}

void ABaseCharacter::Die()
{
	Destroy();
}
