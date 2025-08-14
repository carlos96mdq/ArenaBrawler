// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDBase.h"
#include "UI/PlayerHUDWidget.h"

AHUDBase::AHUDBase()
{
    static ConstructorHelpers::FClassFinder<UPlayerHUDWidget> PlayerHUDWidgetClass(TEXT("/Game/UI/WBP_PlayerHUD.WBP_PlayerHUD_C"));
    if (PlayerHUDWidgetClass.Class != NULL)
    {
        PlayerHUDClass = PlayerHUDWidgetClass.Class;
    }
}

void AHUDBase::BeginPlay()
{
    Super::BeginPlay();

    if (IsValid(PlayerHUDClass))
    {
        PlayerHUD = CreateWidget<UPlayerHUDWidget>(GetWorld(), PlayerHUDClass);
        if (PlayerHUD)
        {
            PlayerHUD->AddToViewport();
        }
    }
}

void AHUDBase::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	if (PlayerHUD)
	{
		PlayerHUD->UpdatePlayerHealth(CurrentHealth, MaxHealth);
	}
}