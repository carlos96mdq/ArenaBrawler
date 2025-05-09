// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class ARENABRAWLER_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Walk and run speed to set the player MaxWalkSpeed when sprinting */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float RunSpeed = 1000;

	/** Attributes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes, meta = (AllowPrivateAccess = "true"))
	float Health = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes, meta = (AllowPrivateAccess = "true"))
	float BasicAttackRange = 100.0f;

	int CurrentAttack = 0;

	/** Animation Montages */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ReceiveDamageMontage;

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Animation functions
	UFUNCTION()
	virtual void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPayload) {};

	// Attack functions
	virtual void AttackStarts(int Attack) {};
	virtual void AttackDealsDamage() {};
	virtual void AttackEnded() {};
	virtual void ReceiveDamage(float Amount);
	virtual void Die();
};
