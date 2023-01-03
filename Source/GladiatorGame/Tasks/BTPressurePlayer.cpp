// Fill out your copyright notice in the Description page of Project Settings.


#include "BTPressurePlayer.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GladiatorGame/AIEnemyController.h"
#include "GladiatorGame/PlayerCharacter.h"

EBTNodeResult::Type UBTPressurePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIEnemyController* aiController = Cast<AAIEnemyController>(OwnerComp.GetAIOwner());

	if(aiController)
	{
		if(aiController->blackboardComp->GetValueAsBool("CanPressure"))
		{
			AEnemy* enemy = aiController->enemy;
			APlayerCharacter* player = enemy->player;
			
			if(enemy->rangeTolerance < FVector::Distance(enemy->GetActorLocation() , player->GetActorLocation()))
			{
				aiController->MoveToLocation(player->GetActorLocation());
			}
			
		}
		return EBTNodeResult::Succeeded;
	}
	else
	{
		DebugError("No ai controller abort Task :: PressurePlayer");
		return EBTNodeResult::Failed;
	}
	
}
