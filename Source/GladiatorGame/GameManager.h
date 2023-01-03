// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "PlayerCharacter.h"

#include "GameManager.generated.h"

UCLASS()
class GLADIATORGAME_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

	APlayerCharacter* playerRef;
	TArray<AEnemy*> currentEnemies;

	UPROPERTY(EditDefaultsOnly, Category = "Instanciate")
	TSubclassOf<AEnemy> enemyBP;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	class ASoundManager* soundManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemies")
	int enemyMax = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemies")
	int enemyCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemies")
	int startingDistance = 600;

	void RemoveEnemy(AEnemy* enemy);

	void StartGame();
	
	void WinScreen();

	/* component for the timer before the game actually start */
	UPROPERTY(EditAnywhere ,BlueprintReadWrite, Category = "CoolDown")
	class UCoolDownComponent* cooldDownStartGame;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void StartMainMenu();
	void StartGameTimer();
	
	bool firstStartDone = false;
	
	AGameHUD* gameHUD;
	APlayerController* controller;
};
