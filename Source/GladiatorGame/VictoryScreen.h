// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VictoryScreen.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API UVictoryScreen : public UUserWidget
{
	GENERATED_BODY()

public :

	UPROPERTY(BlueprintReadWrite , meta = (BindWidget))
	class UButton* Continue;

	UPROPERTY(BlueprintReadWrite , meta = (BindWidget))
	class UButton* Quit;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextScore;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnQuit();

	UFUNCTION()
	void OnContinue();

	UFUNCTION()
	void UpdateScoreText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameManager")
	class AGameManager* gameManager;
	
private :
	TWeakObjectPtr<class APlayerCharacter> player;

	TArray<AActor*> gameManagerList;
	TArray<AActor*> enemies; 
};
