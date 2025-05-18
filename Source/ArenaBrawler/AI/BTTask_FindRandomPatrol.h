// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindRandomPatrol.generated.h"

/**
 * 
 */
UCLASS()
class ARENABRAWLER_API UBTTask_FindRandomPatrol : public UBTTaskNode
{
	GENERATED_BODY()

	float SearchRadius = 2000.f;
	
public:
	UBTTask_FindRandomPatrol();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
