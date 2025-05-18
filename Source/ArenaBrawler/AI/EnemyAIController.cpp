// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "Characters/BaseCharacter.h"

DEFINE_LOG_CATEGORY(LogEnemyAiController);

AEnemyAIController::AEnemyAIController()
{
    bAttachToPawn = true;

    // Assign BT from BP
    static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAsset(TEXT("/Game/AI/BT_EnemyLogic"));
    if (BTAsset.Succeeded())
    {
        BehaviorTree = BTAsset.Object;
    }

    // Sight config
    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
    
    UAISenseConfig_Sight *SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 600.f;
    SightConfig->LoseSightRadius = 800.f;
    SightConfig->PeripheralVisionAngleDegrees = 90.f;
    SightConfig->SetMaxAge(5.f);
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = false;

    PerceptionComponent->ConfigureSense(*SightConfig);
    PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

    PerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &AEnemyAIController::OnTargetPerceived);

    SetGenericTeamId(FGenericTeamId(2));
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (ABaseCharacter* PossesedCharacter = Cast<ABaseCharacter>(InPawn))
    {
        SetGenericTeamId(PossesedCharacter->GetGenericTeamId());
    }

    if (BehaviorTree)
    {
        RunBehaviorTree(BehaviorTree);
    }
}

void AEnemyAIController::OnTargetPerceived(AActor* Actor, FAIStimulus Stimulus)
{
    UE_LOG(LogEnemyAiController, Display, TEXT("'%s' perceived something from Actor %s"), *GetNameSafe(this), *GetNameSafe(Actor));

    // This function is called when the AI perceives a new Pawn OR an already perceived Pawn get out of range
    if (Stimulus.WasSuccessfullySensed())
    {
        UE_LOG(LogEnemyAiController, Display, TEXT("and it was successfully sensed!"));
        GetBlackboardComponent()->SetValueAsObject("TargetActor", Actor);
    }
    else if (GetBlackboardComponent()->GetValueAsObject("TargetActor") == Actor)
    {
        UE_LOG(LogEnemyAiController, Display, TEXT("and it was the target going out of range!"));
        GetBlackboardComponent()->ClearValue("TargetActor");
    }
}