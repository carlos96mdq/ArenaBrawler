// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "AbilitySystemInterface.h"
#include "BaseCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCharacter, Log, All);

class UBrawlerAbilitySystemComponent;
class UBrawlerAttributeSet;
class UGameplayAbility;
class UGameplayEffect;
struct FOnAttributeChangeData;

UCLASS()
class ARENABRAWLER_API ABaseCharacter : public ACharacter, public IGenericTeamAgentInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Walk and run speed to set the player MaxWalkSpeed when sprinting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float RunSpeed = 1000.f;

	// Gameplay Abilities System
	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	UBrawlerAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	const UBrawlerAttributeSet* AttributeSet;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<UGameplayAbility> MeleeAttackAbility;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<UGameplayEffect> InitialGameplayEffects;
	void OnHealthChanged(const FOnAttributeChangeData& Data);
	int CurrentAttack = 0;

	// Animation Montages
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ReceiveDamageMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* BasicAttackMontage;

	// AI detection
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	uint8 TeamID = 0; // Default 0 = neutral

	// DEPRECATED
	//UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	//float CurrentHealth;
	//UFUNCTION()
	//void OnRep_CurrentHealth();

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called to get the Character Gameplay Abilities System
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// AI detection
	virtual FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(TeamID); }

	// Animation
	void PlayMontage(int MontageId);
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayMontage(UAnimMontage* Montage);


	// Combat
	virtual void AttackStarts(int Attack); // Called in client to start the player's attack
	virtual void AttackEnded();
	virtual void Die();

	//DEPRECATED
	//UFUNCTION(Server, Reliable)
	//void ServerAttack(int Attack);	// RCP. Called by Client in Server to start the attack logic and call the animation in all clients
	//virtual void AttackDealsDamage();
	//float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
