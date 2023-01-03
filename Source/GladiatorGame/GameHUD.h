// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CountDownScreen.h"
#include "GameOverScreen.h"
#include "HealthBar.h"
#include "MainMenu.h"
#include "VictoryScreen.h"
#include "GameFramework/HUD.h"
#include "Components/WidgetComponent.h"


#include "GameHUD.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API AGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	AGameHUD();

	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> healthBarClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> pauseMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> gameOverScreenClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> victoryScreenClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> mainMenuClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> countDownClass;
	
	UCountDownScreen* countDown;
	
	UMainMenu* mainMenu;
	
	UVictoryScreen* victoryScreen;
	
	UHealthBar* healthBar;

	UPauseMenu* pauseMenu;

	UGameOverScreen* gameOverScreen;
};