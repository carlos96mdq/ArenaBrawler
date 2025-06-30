// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_MeleeAttack.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMeleeAttack, Log, All);

/**
 * 
 */
UCLASS()
class ARENABRAWLER_API UGA_MeleeAttack : public UGameplayAbility
{
	GENERATED_BODY()

public:
    UGA_MeleeAttack();

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Ability")
    TSubclassOf<UGameplayEffect> DamageEffectClass;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* AttackAnimMontage;

    void ApplyDamage();
    void CleanPlayerData();

    UFUNCTION()
    void OnMontageCompleted();
    UFUNCTION()
    void OnMontageCancelled();
    UFUNCTION()
    void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPayload);
};
