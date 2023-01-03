// Fill out your copyright notice in the Description page of Project Settings.


#include "BTAttackPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../AIEnemyController.h"
#include "GladiatorGame/PlayerCharacter.h"


EBTNodeResult::Type UBTAttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIEnemyController* aiController = Cast<AAIEnemyController>(OwnerComp.GetAIOwner());
	

	if(aiController)
	{
		AEnemy* enemy = aiController->enemy;
		APlayerCharacter* player = enemy->player;
		
		aiController->MoveToLocation(player->GetActorLocation());

		if(FVector::Distance(enemy->GetActorLocation(), player->GetActorLocation()) <= enemy->rangeAttack)
		{
			enemy->Attack();
		}
		
		return EBTNodeResult::Succeeded;
	}
	else
	{
		DebugError("Ai Controller not set abort Task :: AttackPlayer")
		return EBTNodeResult::Failed;
	}
	
}
