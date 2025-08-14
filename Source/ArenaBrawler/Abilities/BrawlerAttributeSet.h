// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BrawlerAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_LOG_CATEGORY_EXTERN(LogAttributeSet, Log, All);

/**
 * 
 */
UCLASS()
class ARENABRAWLER_API UBrawlerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
    FGameplayAttributeData Health = 0;
    UPROPERTY(BlueprintReadOnly, Category = "Attributes", Replicated)
    FGameplayAttributeData MaxHealth = 0;
    UPROPERTY(BlueprintReadOnly, Category = "Attributes")
    FGameplayAttributeData Damage = 0;
    UPROPERTY(BlueprintReadOnly, Category = "Attributes")
    FGameplayAttributeData AttackPower = 0;
    UPROPERTY(BlueprintReadOnly, Category = "Attributes")
    FGameplayAttributeData MoveSpeed = 0;

public:
    UBrawlerAttributeSet() {};

    // Replication. Add the attributes that need replication
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    ATTRIBUTE_ACCESSORS(UBrawlerAttributeSet, Health)
    ATTRIBUTE_ACCESSORS(UBrawlerAttributeSet, MaxHealth)
    ATTRIBUTE_ACCESSORS(UBrawlerAttributeSet, Damage)
    ATTRIBUTE_ACCESSORS(UBrawlerAttributeSet, AttackPower)
    ATTRIBUTE_ACCESSORS(UBrawlerAttributeSet, MoveSpeed)

    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

    UFUNCTION()
    void OnRep_Health(const FGameplayAttributeData& OldHealth);
};
