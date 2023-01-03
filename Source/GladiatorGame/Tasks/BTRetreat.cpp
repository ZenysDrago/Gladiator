// Fill out your copyright notice in the Description page of Project Settings.


#include "BTRetreat.h"

#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../AIEnemyController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GladiatorGame/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTRetreat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIEnemyController* AIController = Cast<AAIEnemyController>(OwnerComp.GetAIOwner());

	if(AIController)
	{
		UBlackboardComponent* blackboardComp = AIController->GetBlackboardComponent();
		FVector dir = blackboardComp->GetValueAsVector("DirNormal");
		const FVector adjust = blackboardComp->GetValueAsVector("DirAdjust");

		dir.Normalize();
		FVector newDir = dir + adjust ;
		newDir.Normalize();
		
		FVector newVelocity = newDir * 100 ;
		newVelocity.Z = 0;
		
		AIController->MoveToLocation(newVelocity + AIController->enemy->GetActorLocation());
		
		return EBTNodeResult::Succeeded;
	}
	else
	{
		DebugError("No ai controller abort Task :: FindDirection");
		return EBTNodeResult::Failed;
	}

}
