// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BTSetBlackBoardValue.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API UBTSetBlackBoardValue : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere , Category = "BoolValue")
	bool boolToUse;

	UPROPERTY(EditAnywhere , Category = "BoolValue")
	FBlackboardKeySelector keyNameBool;
	
	UPROPERTY(EditAnywhere, Category = "FVector Value")
	FVector vectorToUse;

	UPROPERTY(EditAnywhere, Category = "FVector Value")
	FBlackboardKeySelector keyNameVector;
};
