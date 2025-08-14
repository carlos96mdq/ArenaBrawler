// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDBase.generated.h"

class UPlayerHUDWidget;

/**
 * 
 */
UCLASS()
class ARENABRAWLER_API AHUDBase : public AHUD
{
	GENERATED_BODY()
	
public:

    AHUDBase();
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "HUD")
    TSubclassOf<UPlayerHUDWidget> PlayerHUDClass;
    UPROPERTY(BlueprintReadOnly)
    UPlayerHUDWidget* PlayerHUD;

    UFUNCTION()
    void UpdateHealth(float CurrentHealth, float MaxHealth);
};
