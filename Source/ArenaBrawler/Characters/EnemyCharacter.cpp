// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


AEnemyCharacter::AEnemyCharacter()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	
	// Set team for AI detection
	TeamID = 2;
}