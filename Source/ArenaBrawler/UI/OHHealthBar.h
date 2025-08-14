// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OHHealthBar.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class ARENABRAWLER_API UOHHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UProgressBar* HealthBar;

	void UpdateHealth(float CurrentHealth, float MaxHealth);
};
