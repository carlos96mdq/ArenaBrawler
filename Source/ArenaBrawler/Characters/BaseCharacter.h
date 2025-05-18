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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes, meta = (AllowPrivateAccess = "true"))
	float Health = 100.f;
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

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// AI detection
	virtual FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(TeamID); }

	// Animation functions
	UFUNCTION()
	virtual void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPayload);

	// Attack functions
	virtual void AttackStarts(int Attack);
	virtual void AttackDealsDamage();
	virtual void AttackEnded();
	virtual void ReceiveDamage(float Amount);
	virtual void Die();
};
