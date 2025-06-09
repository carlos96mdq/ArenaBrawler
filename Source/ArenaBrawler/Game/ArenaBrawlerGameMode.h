// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ArenaBrawlerGameMode.generated.h"


UCLASS(minimalapi)
class AArenaBrawlerGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	TSubclassOf<class AEnemyCharacter> EnemyClass;

public:
	AArenaBrawlerGameMode();
};



