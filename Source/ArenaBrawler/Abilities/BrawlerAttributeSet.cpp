// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/BrawlerAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "Characters/BaseCharacter.h"

DEFINE_LOG_CATEGORY(LogAttributeSet);

void UBrawlerAttributeSet::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicate Health attribute
	DOREPLIFETIME(UBrawlerAttributeSet, Health);
	DOREPLIFETIME(UBrawlerAttributeSet, MaxHealth);
}

void UBrawlerAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
}

void UBrawlerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// Apply damage to Health attribute
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Get the damage done by this Gameplay Effect, modify it if needed and clean the damage attribute for future effects
		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		// Modify damage done if needed

		// Apply damage to Health attribute
		if (LocalDamageDone > 0.f)
		{
			const float NewHealth = GetHealth() - LocalDamageDone;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			// Play damage montage on character
			ABaseCharacter* Owner = Cast<ABaseCharacter>(GetOwningActor());
			if (Owner)
			{
				Owner->PlayMontage(1);
			}
		}
	}
}

void UBrawlerAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBrawlerAttributeSet, Health, OldHealth);
}