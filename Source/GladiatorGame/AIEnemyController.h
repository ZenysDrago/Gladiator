// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Enemy.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API AAIEnemyController : public AAIController
{
	GENERATED_BODY()

public :
	AAIEnemyController();
	
	UPROPERTY(EditAnywhere, Category = AI)
	UBehaviorTreeComponent* behaviorTreeComp;

	UPROPERTY(EditAnywhere, Category = AI)
	UBlackboardComponent* blackboardComp;
	
	class UAISenseConfig_Sight* sight;
	
	virtual void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;
	
	AEnemy* enemy;
protected:
	UPROPERTY(VisibleAnywhere)
	UAIPerceptionComponent* AIPerceptionComp;

	UFUNCTION()
	void SetupPerception();

	UFUNCTION()
	void OnTargetDetected(AActor* actor , FAIStimulus const stimulus);
};
