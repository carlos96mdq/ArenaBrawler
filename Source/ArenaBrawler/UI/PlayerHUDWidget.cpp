// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUDWidget.h"
#include "Components/ProgressBar.h"

void UPlayerHUDWidget::UpdatePlayerHealth(float CurrentHealth, float MaxHealth)
{
    if (PlayerHealthBar && MaxHealth > 0)
    {
        float Percent = CurrentHealth / MaxHealth;
        PlayerHealthBar->SetPercent(FMath::Clamp(Percent, 0, MaxHealth));
    }
}
