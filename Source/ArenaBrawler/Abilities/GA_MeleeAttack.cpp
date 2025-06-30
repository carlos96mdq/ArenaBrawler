// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/GA_MeleeAttack.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/AbilityTask_PlayMontageAndWaitEv.h"
#include "Characters/BaseCharacter.h"

DEFINE_LOG_CATEGORY(LogMeleeAttack);

UGA_MeleeAttack::UGA_MeleeAttack()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    // Add GameplayEffect
    static ConstructorHelpers::FClassFinder<UGameplayEffect> GEDamageClass(TEXT("/Game/Abilities/GE_Damage.GE_Damage_C"));
    if (GEDamageClass.Class != NULL)
    {
        DamageEffectClass = GEDamageClass.Class;
    }
    
    // Add Montage
    static ConstructorHelpers::FObjectFinder<UAnimMontage> AnimMontageObj(TEXT("/Game/Characters/Mannequins/Animations/Manny/AM_BasicAttack.AM_BasicAttack"));
    if (AnimMontageObj.Succeeded())
    {
        AttackAnimMontage = AnimMontageObj.Object;
    }
}

void UGA_MeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        return;
    }

    ACharacter* Player = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (Player && AttackAnimMontage)
    {
        UAbilityTask_PlayMontageAndWaitEv* MontageTask = UAbilityTask_PlayMontageAndWaitEv::CreatePlayMontageAndWaitEvProxy(this, NAME_None, AttackAnimMontage);

        MontageTask->OnBlendOut.AddDynamic(this, &UGA_MeleeAttack::OnMontageCompleted);
        MontageTask->OnCompleted.AddDynamic(this, &UGA_MeleeAttack::OnMontageCompleted);
        MontageTask->OnInterrupted.AddDynamic(this, &UGA_MeleeAttack::OnMontageCancelled);
        MontageTask->OnCancelled.AddDynamic(this, &UGA_MeleeAttack::OnMontageCancelled);
        MontageTask->OnPlayMontageNotifyBegin.AddDynamic(this, &UGA_MeleeAttack::OnMontageNotifyBegin);

        MontageTask->ReadyForActivation();
    }
    else
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
    }
}

void UGA_MeleeAttack::OnMontageCompleted()
{
    // Client-specific functionality
    if (IsLocallyControlled())
    {
        CleanPlayerData();
    }
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_MeleeAttack::OnMontageCancelled()
{
    if (IsLocallyControlled())
    {
        CleanPlayerData();
    }
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_MeleeAttack::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPayload)
{
    // Server-specific functionality
    if (HasAuthority(&CurrentActivationInfo))
    {
        if (NotifyName == "DealDamage")
        {
            ApplyDamage();
        }
    }
}

void UGA_MeleeAttack::ApplyDamage()
{
    if (!DamageEffectClass)
    {
        return;
    }
    AActor* SourceActor = GetAvatarActorFromActorInfo();

    // Line trace u otro método
    FHitResult HitResult;
    FVector Start = SourceActor->GetActorLocation();
    FVector End = Start + SourceActor->GetActorForwardVector() * 150.f;
    FCollisionShape Shape = FCollisionShape::MakeSphere(1.0f);

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(SourceActor);

    if (GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_Pawn, Shape, Params))
    {
        ABaseCharacter* HitCharacter = Cast<ABaseCharacter>(HitResult.GetActor());
        if (HitCharacter)
        {
            FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());
            if (SpecHandle.IsValid())
            {
                UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitCharacter);
                if (TargetASC)
                {
                    TargetASC->ApplyGameplayEffectToSelf(DamageEffectClass->GetDefaultObject<UGameplayEffect>(), 0.f, TargetASC->MakeEffectContext());
                }
            }
        }
    }
}

void UGA_MeleeAttack::CleanPlayerData()
{
    ABaseCharacter* Character = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
    if (Character)
    {
        Character->AttackEnded();
    }
}