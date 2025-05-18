// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogEnemyAiController, Log, All);

struct FAIStimulus;

/**
 * 
 */
UCLASS()
class ARENABRAWLER_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;

    UFUNCTION()
    void OnTargetPerceived(AActor* Actor, FAIStimulus Stimulus);

    UPROPERTY(EditDefaultsOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    UBehaviorTree* BehaviorTree;

public:
    AEnemyAIController();
};
