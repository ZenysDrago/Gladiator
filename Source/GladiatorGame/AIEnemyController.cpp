// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemyController.h"

#include "PlayerCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GamePlayStatics.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

AAIEnemyController::AAIEnemyController()
{
	behaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));

	SetupPerception();
}

void AAIEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	enemy = Cast<AEnemy>(InPawn);

	if(enemy != nullptr && enemy->behaviorTree->BlackboardAsset != nullptr)
	{
		
		UseBlackboard(enemy->behaviorTree->BlackboardAsset, blackboardComp);

		blackboardComp->SetValueAsObject("SelfActor", enemy);

		FVector spawnPos = enemy->GetActorLocation();
		blackboardComp->SetValueAsVector("SpawnPosition", spawnPos);
		
		behaviorTreeComp->StartTree(*enemy->behaviorTree);
	}
}

void AAIEnemyController::BeginPlay()
{
	Super::BeginPlay();
	
	if(behaviorTreeComp->IsRunning())
	{
		APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
		blackboardComp->SetValueAsObject("Player", player);
	}
}

void AAIEnemyController::SetupPerception()
{
	sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception component")));
	sight->SightRadius = 5000.f;
	sight->LoseSightRadius = sight->SightRadius + 100.f;
	sight->PeripheralVisionAngleDegrees = 60.f;
	sight->SetMaxAge(0);
	sight->AutoSuccessRangeFromLastSeenLocation = 0;
	sight->DetectionByAffiliation.bDetectNeutrals = true;

	
	GetPerceptionComponent()->ConfigureSense(*sight);
	GetPerceptionComponent()->SetDominantSense(*sight->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAIEnemyController::OnTargetDetected);
	
}

void AAIEnemyController::OnTargetDetected(AActor* actor, FAIStimulus const stimulus)
{
	if(auto const ch = Cast<APlayerCharacter>(actor))
		blackboardComp->SetValueAsBool("PlayerOnSight", stimulus.WasSuccessfullySensed());
}



