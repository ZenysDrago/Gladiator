// Fill out your copyright notice in the Description page of Project Settings.


#include "BTCheckRange.h"
#include "Kismet/GameplayStatics.h"
#include "../Enemy.h"
#include "../PlayerCharacter.h"
#include "../AIEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"



EBTNodeResult::Type UBTCheckRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TArray<AActor*> entities;
	asBeenSetTrue = false;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), entities);
	entities.Add(Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0)));

	AAIEnemyController* AIController = Cast<AAIEnemyController>(OwnerComp.GetAIOwner());
	UBlackboardComponent* blackboardComp = AIController->GetBlackboardComponent();

	AEntity* selfEntity = Cast<AEntity>(blackboardComp->GetValueAsObject("SelfActor"));
	for (auto actor : entities)
	{
		if(actor == nullptr)
		{
			DebugError("Actor in range is not an entity");
			return EBTNodeResult::Failed;
		}
		
		AEntity* entity = Cast<AEntity>(actor);
		if(entity != selfEntity) 
		{
			FVector posEntity	= entity->GetActorLocation();
			FVector selfPos		= selfEntity->GetActorLocation();

			AEnemy* selfEnemy = Cast<AEnemy>(blackboardComp->GetValueAsObject("SelfActor"));
			AEnemy* target = Cast<AEnemy>(entity);
			
			float range;
			if(target)	{ range = selfEnemy->enemyTolerance; }
			else		{ range = selfEnemy->rangeTolerance; }
			
			if(FVector::Distance(posEntity , selfPos) <= range && !entity->healthManager->isDead)
			{
				blackboardComp->SetValueAsBool("IsToClose", true);
				asBeenSetTrue = true;
				FVector dir =  selfPos - posEntity;
				dir.Normalize();
				blackboardComp->SetValueAsVector("DirNormal", dir);
			}
		}
	}

	if(!asBeenSetTrue)
	{
		blackboardComp->SetValueAsBool("IsToClose", false);	
	}
	return EBTNodeResult::Succeeded;
}
