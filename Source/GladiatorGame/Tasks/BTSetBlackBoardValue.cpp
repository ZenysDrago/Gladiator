// Fill out your copyright notice in the Description page of Project Settings.


#include "BTSetBlackBoardValue.h"

#include <stdbool.h>

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "GladiatorGame/AIEnemyController.h"

EBTNodeResult::Type UBTSetBlackBoardValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIEnemyController* aiController = Cast<AAIEnemyController>(OwnerComp.GetAIOwner());
	
	if(aiController)
	{

		aiController->blackboardComp->SetValueAsBool(keyNameBool.SelectedKeyName, boolToUse);

		aiController->blackboardComp->SetValueAsVector(keyNameVector.SelectedKeyName, vectorToUse);
		
		return EBTNodeResult::Succeeded;
	}
	else
	{
		DebugError("AIController not valid abort Task :: SetBlackBoardValue");
		return EBTNodeResult::Failed;
	}
	
}
