// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArenaBrawlerGameMode.h"
#include "Controllers/PlayerControllerBase.h"
#include "Characters/EnemyCharacter.h"
#include "UObject/ConstructorHelpers.h"

AArenaBrawlerGameMode::AArenaBrawlerGameMode()
{
	// Set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/BP_PlayerCharacter.BP_PlayerCharacter_C"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

    // Set default pawn class to our Blueprinted enemy
    static ConstructorHelpers::FClassFinder<APawn> EnemyPawnBPClass(TEXT("/Game/Characters/BP_EnemyCharacter.BP_EnemyCharacter_C"));
    if (EnemyPawnBPClass.Class != NULL)
    {
        EnemyClass = EnemyPawnBPClass.Class;
    }

	PlayerControllerClass = APlayerControllerBase::StaticClass();
}

void AArenaBrawlerGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Spawn an enemy in World
    //if (EnemyClass)
    //{
    //    FVector SpawnLocation(2500.f, 1290.f, 88.f);
    //    FRotator SpawnRotation = FRotator::ZeroRotator;
    //    GetWorld()->SpawnActor<AEnemyCharacter>(EnemyClass, SpawnLocation, SpawnRotation);
    //}
}
