// Fill out your copyright notice in the Description page of Project Settings.


#include "BTFindDirection.h"

#include "DrawDebugHelpers.h"
#include "../AIEnemyController.h"
#include "../PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTFindDirection::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIEnemyController* aiController = Cast<AAIEnemyController>(OwnerComp.GetAIOwner());

	if(!aiController)
	{
		DebugError("No ai controller abort Task :: FindDirection");
		return EBTNodeResult::Failed;
	}
		
	APlayerCharacter* player = aiController->enemy->player;

	FVector playerPos = player->GetActorLocation();
	FVector Start = aiController->enemy->GetActorLocation();
	FVector EndRight = Start + (aiController->enemy->GetActorRightVector()   *    300.f );
	FVector EndLeft  = Start + (aiController->enemy->GetActorRightVector()   * (- 300.f));
	FVector EndBack  = Start + (aiController->enemy->GetActorForwardVector() * (- 300.f));

	//TODO
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(aiController->enemy);
	//Ignore actors for every enemy
	
	FHitResult outHitRight;
	FHitResult outHitLeft;
	FHitResult outHitBack;

	//DEBUG
	// DrawDebugLine(GetWorld(), Start , EndRight, FColor::Cyan, false, 1 , 0 , 1);
	// DrawDebugLine(GetWorld(), Start , EndLeft, FColor::Yellow, false, 1 , 0 , 1);
	// DrawDebugLine(GetWorld(), Start , EndBack, FColor::Purple, false, 1 , 0 , 1);

	bool asHitRight = GetWorld()->LineTraceSingleByChannel(outHitRight, Start, EndRight, ECC_Visibility, collisionParams, FCollisionResponseParams());
	bool asHitLeft  = GetWorld()->LineTraceSingleByChannel(outHitLeft , Start, EndLeft , ECC_Visibility, collisionParams, FCollisionResponseParams());
	bool asHitBack  = GetWorld()->LineTraceSingleByChannel(outHitBack , Start, EndBack , ECC_Visibility, collisionParams, FCollisionResponseParams());

	FVector dirAdjust = FVector::ZeroVector;
	if(asHitRight)
	{
		if(!outHitRight.GetActor())
		{
			DebugError("No actor hit on right , :: abort task Find Direction");
		}
		dirAdjust += outHitRight.ImpactNormal;
	}
	if(asHitBack)
	{
		if(!outHitBack.GetActor())
		{
			DebugError("No actor hit on back  , :: abort task Find Direction")
		}
		dirAdjust += outHitBack.ImpactNormal;
	}
	if(asHitLeft)
	{
		if(!outHitLeft.GetActor())
		{
			DebugError("No actor hit on left  , :: abort task Find Direction");
		}
		dirAdjust += outHitLeft.ImpactNormal;
	}
	aiController->blackboardComp->SetValueAsVector("DirAdjust", dirAdjust.GetSafeNormal());

	return EBTNodeResult::Succeeded;
		
}
