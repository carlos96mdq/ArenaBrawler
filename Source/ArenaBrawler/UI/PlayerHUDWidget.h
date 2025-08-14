// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class ARENABRAWLER_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UProgressBar* PlayerHealthBar;

	void UpdatePlayerHealth(float CurrentHealth, float MaxHealth);
};
