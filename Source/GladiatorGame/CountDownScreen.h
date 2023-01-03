// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CountDownScreen.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API UCountDownScreen : public UUserWidget
{
	GENERATED_BODY()

public :
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* countDownText;

	class UCoolDownComponent* cooldDownGame;

	UFUNCTION()
	void UpdateText();

	void OnStart(UCoolDownComponent* cooldDownGameRef);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
