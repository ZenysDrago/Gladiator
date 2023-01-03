// Fill out your copyright notice in the Description page of Project Settings.


#include "BTFindPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GladiatorGame/AIEnemyController.h"

EBTNodeResult::Type UBTFindPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AAIEnemyController* aiController = Cast<AAIEnemyController>(OwnerComp.GetAIOwner());
	AEnemy* selfActor = aiController->enemy;
	
	if(aiController)
	{
		const FVector right = selfActor->GetActorRightVector();
		const FVector adjust = aiController->GetBlackboardComponent()->GetValueAsVector("DirAdjust");
		FVector dir = right + adjust;
		dir.Z = 0;
		
		const FVector destination = selfActor->GetActorLocation() + ( dir * 50.f);
		
		aiController->MoveToLocation(destination);
	}
	else
	{
		DebugError("No ai controller abort Task :: FindPlayer");
		return EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::Succeeded;
}
