// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_AttackTarget.h"
#include "AIController.h"
#include "Characters/EnemyCharacter.h"


UBTTask_AttackTarget::UBTTask_AttackTarget()
{
    NodeName = "Attack Target";
}

EBTNodeResult::Type UBTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    AEnemyCharacter* Character = Cast<AEnemyCharacter>(AIController->GetPawn());
    if (!Character) return EBTNodeResult::Failed;

    Character->AttackStarts(1);
    return EBTNodeResult::Succeeded;
}