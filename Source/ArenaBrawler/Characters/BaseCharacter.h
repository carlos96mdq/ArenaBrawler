// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "BaseCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCharacter, Log, All);

UCLASS()
class ARENABRAWLER_API ABaseCharacter : public ACharacter, public IGenericTeamAgentInterface
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

	// Attributes
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Attributes)
	float MaxHealth = 100.f;
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	float CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes, meta = (AllowPrivateAccess = "true"))
	float BasicAttackRange = 100.f;
	int CurrentAttack = 0;

	// Animation Montages
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ReceiveDamageMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* BasicAttackMontage;

	// AI detection
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	uint8 TeamID = 0; // Default 0 = neutral

	// Events
	void OnHealthUpdate();	// Called in server when health is modified, and in client when it is replicated from server

	// Replication
	UFUNCTION()
	void OnRep_CurrentHealth();

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	// Replication. Add the attributes that need replication
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// AI detection
	virtual FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(TeamID); }

	// Animation functions
	UFUNCTION()
	virtual void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPayload);
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayMontage(UAnimMontage* Montage);


	// Attack functions
	virtual void AttackStarts(int Attack); // Called in client to start the player's attack
	UFUNCTION(Server, Reliable)
	void ServerAttack(int Attack);	// RCP. Called by Client in Server to start the attack logic and call the animation in all clients
	virtual void AttackDealsDamage();
	virtual void AttackEnded();

	// Event for taking damage, overridden from APawn
	float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Die();

	// Getters and Setters
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	void SetCurrentHealth(float HealthValue);
};
