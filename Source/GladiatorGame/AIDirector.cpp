// Fill out your copyright notice in the Description page of Project Settings.


#include "AIDirector.h"

#include "CoolDownComponent.h"
#include "GameManager.h"
#include "LocalizationDescriptor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAIDirector::AAIDirector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CoolDownComponent = CreateDefaultSubobject<UCoolDownComponent>(TEXT("CoolDown Component"));
	
}

// Called when the game starts or when spawned
void AAIDirector::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAIDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* If the delay as passed choose a new attacker */
	if(CoolDownComponent->IsOver() && gameManager->enemyCount > 0)
		ChooseAttacker();
}

/* Choose an enemy to attack player */
void AAIDirector::ChooseAttacker()
{	
	for(const AEnemy* enemy : gameManager->currentEnemies)
		if(enemy->isAttacking)
			return;
	
	AEnemy* enemyChoose = gameManager->currentEnemies[FMath::RandRange(0 , gameManager->currentEnemies.Num()-1)];
	
	if(enemyChoose && enemyChoose->coolDownAttack->IsOver() && !enemyChoose->healthManager->isDead)
	{
		/* Get the AI Controller of the enemy */
		AAIEnemyController* controllerOfEnemy = Cast<AAIEnemyController>(enemyChoose->GetController());
		if(!controllerOfEnemy)
		{
			DebugError("Couldn't get the AiController");
			return;
		}

		/* Notify bb that enemy is should attack */
		controllerOfEnemy->GetBlackboardComponent()->SetValueAsBool("IsAttacking" , true);

		/* Set the CD for the Director and set the player as Attacking */
		enemyChoose->isAttacking = true;
		enemyChoose->coolDownAttack->ResetTimer();
		CoolDownComponent->Start();
	}
	else if(!enemyChoose)
		DebugError("Not a valid enemy");
}
