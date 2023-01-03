// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enemy.h"

#include "EnemyHealthBar.generated.h"

/**
 * 
 */

UCLASS( Abstract )
class GLADIATORGAME_API UEnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetOwnerEnemy(AEnemy* enemy);

protected:

	TWeakObjectPtr<AEnemy> ownerEnemy;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* healthBar;

	UFUNCTION()
	void UpdateLife();

};
