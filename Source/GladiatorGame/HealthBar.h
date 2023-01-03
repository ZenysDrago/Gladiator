// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

private:
	TWeakObjectPtr<APlayerCharacter> player;
	
public :
	UPROPERTY(BlueprintReadWrite , meta = (BindWidget))
	class UImage* healthBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* hpText;

	UFUNCTION()
	void UpdateLife();

	virtual void NativeConstruct() override;
};
