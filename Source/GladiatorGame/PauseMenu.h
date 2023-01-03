// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
class APlayerCharacter;

UCLASS()
class GLADIATORGAME_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()
	
public :
	
	UPROPERTY(BlueprintReadWrite , meta = (BindWidget))
	class UButton* Resume;

	UPROPERTY(BlueprintReadWrite , meta = (BindWidget))
	class UButton* Quit;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextScore;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnQuit();

	UFUNCTION()
	void OnResume();

	UFUNCTION()
	void UpdateScoreText();
	
private :
	TWeakObjectPtr<APlayerCharacter> player;
};
