// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public :

	UPROPERTY(BlueprintReadWrite , meta = (BindWidget))
	class UButton* Start;

	UPROPERTY(BlueprintReadWrite , meta = (BindWidget))
	class UButton* Quit;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnQuit();

	UFUNCTION()
	void OnStart();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameManager")
	class AGameManager* gameManager;

private :
	TArray<AActor*> gameManagerList;
};
