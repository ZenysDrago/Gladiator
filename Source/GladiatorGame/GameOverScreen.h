// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverScreen.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API UGameOverScreen : public UUserWidget
{
	GENERATED_BODY()

public :
	UPROPERTY(BlueprintReadWrite , meta = (BindWidget))
	class UButton* Restart;

	UPROPERTY(BlueprintReadWrite , meta = (BindWidget))
	class UButton* Quit;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextScore;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnQuit();

	UFUNCTION()
	void OnRestart();

	UFUNCTION()
	void UpdateScoreText();

private :
		TWeakObjectPtr<class APlayerCharacter> player;
};
