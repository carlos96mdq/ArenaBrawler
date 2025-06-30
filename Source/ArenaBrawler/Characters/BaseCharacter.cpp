// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Abilities/BrawlerAbilitySystemComponent.h"
#include "Abilities/BrawlerAttributeSet.h"
#include "GameFramework/DamageType.h"

DEFINE_LOG_CATEGORY(LogCharacter);

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// GAS
	AbilitySystemComponent = CreateDefaultSubobject<UBrawlerAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UBrawlerAttributeSet>(TEXT("AttributeSet"));
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		const FString NetRoleStr = UEnum::GetValueAsString(GetLocalRole());
		const FString Message = FString::Printf(TEXT("Owner: %s | Role: %s"), *PC->GetName(), *NetRoleStr);
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, Message);
	}

	// Set initial abilities and attributes (using GameplayEffects)
	if (HasAuthority() && AbilitySystemComponent && AttributeSet)
	{
		if (MeleeAttackAbility)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(MeleeAttackAbility, 1, 1));
		}

		if (InitialGameplayEffects)
		{
			AbilitySystemComponent->ApplyGameplayEffectToSelf(InitialGameplayEffects->GetDefaultObject<UGameplayEffect>(), 0.f, AbilitySystemComponent->MakeEffectContext());
		}
	}

	// Bind the Health attribute change delegate to a function (In server AND client)
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &ABaseCharacter::OnHealthChanged);
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
	else
	{
		if (AbilitySystemComponent)
		{
			CurrentAttack = Attack;
			AbilitySystemComponent->TryActivateAbilityByClass(MeleeAttackAbility);
		}
	}
}

void ABaseCharacter::AttackEnded()
{
	CurrentAttack = 0;
}

void ABaseCharacter::Die()
{
	//if (HasAuthority())
	//{
	//	if (UAnimMontage* DeathMontage = DeathMontageReference)
	//	{
	//		PlayAnimMontage(DeathMontage);
	//	}

	//	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//	GetMesh()->SetSimulatePhysics(true);

	//	// Delay para destruir luego
	//	SetLifeSpan(5.0f);
	//}
	Destroy();
}

void ABaseCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	// Client-specific functionality
	if (IsLocallyControlled())
	{
		if (AttributeSet)
		{
			const float Health = AttributeSet->GetHealth();
			const FString NetRoleStr = UEnum::GetValueAsString(GetLocalRole());
			FString Message = FString::Printf(TEXT("You now have %f health remaining (%s)"), Health, *NetRoleStr);
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, Message);
			if (Health <= 0)
			{
				Message = FString::Printf(TEXT("You have been killed (%s)"), *NetRoleStr);
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, Message);
			}
		}
	}

	// Server-specific functionality
	if (GetLocalRole() == ROLE_Authority)
	{
		const float Health = AttributeSet->GetHealth();
		if (Health <= 0)
		{
			Die();
		}
	}
}

void ABaseCharacter::PlayMontage(int MontageId)
{
	switch (MontageId)
	{
	case 0:
		if (BasicAttackMontage)
		{
			MulticastPlayMontage(BasicAttackMontage);
		}
	case 1:
		if (ReceiveDamageMontage)
		{
			MulticastPlayMontage(ReceiveDamageMontage);
		}
	default:
		break;
	}
}

void ABaseCharacter::MulticastPlayMontage_Implementation(UAnimMontage* Montage)
{
	if (Montage && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(Montage);
	}
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// DEPRECATED
//void ABaseCharacter::ServerAttack_Implementation(int Attack)
//{
//	switch (Attack)
//	{
//	case 1:	// Basic attack
//		if (BasicAttackMontage)
//		{
//			MulticastPlayMontage(BasicAttackMontage);	// Play the attack animation of this character in all clients
//		}
//		else
//		{
//			UE_LOG(LogCharacter, Error, TEXT("BasicAttackMontage assets not found in '%s'"), *GetNameSafe(this));
//		}
//		break;
//	default:
//		break;
//	}
//}
//
//void ABaseCharacter::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPayload)
//{
//	// Client-specific functionality
//	if (IsLocallyControlled())
//	{
//		if (NotifyName == "AttackEnded")
//		{
//			AttackEnded();
//		}
//	}
//
//	// Server-specific functionality
//	if (GetLocalRole() == ROLE_Authority)
//	{
//		if (NotifyName == "DealDamage")
//		{
//			AttackDealsDamage();
//		}
//	}
//}
//
//void ABaseCharacter::AttackDealsDamage()
//{
//	//TODO For now we don't care about the attack type. Just do damage.
//	FVector Start = GetActorLocation();
//	FVector End = Start + GetActorForwardVector() * BasicAttackRange;
//	FCollisionShape Shape = FCollisionShape::MakeSphere(1.0f);
//
//	FHitResult HitResult;
//	FCollisionQueryParams Params;
//	Params.AddIgnoredActor(this);
//
//	bool bHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_Pawn, Shape, Params);
//	//DrawDebugCylinder(GetWorld(), Start, End, 1.0f, 5, FColor::Red, true, 10.0f);
//
//	if (bHit)
//	{
//		ABaseCharacter* HitCharacter = Cast<ABaseCharacter>(HitResult.GetActor());
//		if (HitCharacter)
//		{
//			//HitCharacter->ReceiveDamage(20);
//			TSubclassOf<class UDamageType> DamageType = UDamageType::StaticClass();	// To be implemented in the future
//			UGameplayStatics::ApplyDamage(HitCharacter, 20.f, GetInstigator()->Controller, this, DamageType);
//		}
//	}
//}
//float ABaseCharacter::TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
//{
//	if (ReceiveDamageMontage)
//	{
//		MulticastPlayMontage(ReceiveDamageMontage);
//	}
//
//	float DamageApplied = CurrentHealth - DamageTaken;
//	SetCurrentHealth(DamageApplied);
//	return DamageApplied;
//}
//void ABaseCharacter::OnRep_CurrentHealth()
//{
//	OnHealthUpdate();
//}
//void ABaseCharacter::OnHealthUpdate()
//{
//	// Client-specific functionality
//	if (IsLocallyControlled())
//	{
//		const FString NetRoleStr = UEnum::GetValueAsString(GetLocalRole());
//		FString Message = FString::Printf(TEXT("You now have %f health remaining (%s)"), CurrentHealth, *NetRoleStr);
//		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, Message);
//
//		if (CurrentHealth <= 0)
//		{
//			Message = FString::Printf(TEXT("You have been killed (%s)"), *NetRoleStr);
//			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, Message);
//		}
//	}
//
//	// Server-specific functionality
//	if (GetLocalRole() == ROLE_Authority)
//	{
//		const FString NetRoleStr = UEnum::GetValueAsString(GetLocalRole());
//		const FString Message = FString::Printf(TEXT("%s now has %f health remaining (%s)"), *GetNameSafe(this), CurrentHealth, *NetRoleStr);
//		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, Message);
//
//		if (CurrentHealth <= 0)
//		{
//			Die();
//		}
//	}
//
//	// Functions that occur on all machines.
//
//}
//void ABaseCharacter::SetCurrentHealth(float HealthValue)
//{
//	if (GetLocalRole() == ROLE_Authority)
//	{
//		CurrentHealth = FMath::Clamp(HealthValue, 0.f, MaxHealth);
//		OnHealthUpdate();
//	}
//}