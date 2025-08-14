// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OHHealthBar.h"
#include "Components/ProgressBar.h"

void UOHHealthBar::UpdateHealth(float CurrentHealth, float MaxHealth)
{
    if (HealthBar && MaxHealth > 0)
    {
        float Percent = CurrentHealth / MaxHealth;
        HealthBar->SetPercent(FMath::Clamp(Percent, 0, MaxHealth));
    }
}