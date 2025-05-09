// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArenaBrawlerGameMode.h"
#include "Characters/ArenaBrawlerCharacter.h"
#include "Controllers/PlayerControllerBase.h"
#include "UObject/ConstructorHelpers.h"

AArenaBrawlerGameMode::AArenaBrawlerGameMode()
{
	// Set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/BP_PlayerCharacter.BP_PlayerCharacter_C"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	PlayerControllerClass = APlayerControllerBase::StaticClass();
}
