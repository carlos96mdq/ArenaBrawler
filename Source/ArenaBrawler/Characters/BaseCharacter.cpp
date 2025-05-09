// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"

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
