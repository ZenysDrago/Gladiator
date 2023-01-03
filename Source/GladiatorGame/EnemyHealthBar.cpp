// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHealthBar.h"
#include <Components/ProgressBar.h>
#include "../Debug.h"

void UEnemyHealthBar::SetOwnerEnemy(AEnemy* enemy)
{
	ownerEnemy = enemy;
	ownerEnemy->healthManager->updatedLifeDelegate.BindUObject(this, &UEnemyHealthBar::UpdateLife);
	UpdateLife();

}

void UEnemyHealthBar::UpdateLife()
{
	if (!ownerEnemy.IsValid())
		return;

	healthBar->SetPercent(ownerEnemy->healthManager->life / (float)ownerEnemy->healthManager->maxLife);
}
