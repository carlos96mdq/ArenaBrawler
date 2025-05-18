// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindRandomPatrol.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Characters/EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomPatrol::UBTTask_FindRandomPatrol()
{
    NodeName = "Find Random Patrol";
}

EBTNodeResult::Type UBTTask_FindRandomPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    AEnemyCharacter* Character = Cast<AEnemyCharacter>(AIController->GetPawn());
    if (!Character) return EBTNodeResult::Failed;
    
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSys) return EBTNodeResult::Failed;

    FNavLocation DestinationNavLocation;
    if (NavSys->GetRandomReachablePointInRadius(Character->GetActorLocation(), SearchRadius, DestinationNavLocation))
    {
        AIController->GetBlackboardComponent()->SetValueAsVector("PatrolLocation", DestinationNavLocation.Location);
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;
}