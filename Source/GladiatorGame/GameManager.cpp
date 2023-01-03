// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameHUD.h"
#include "SoundManager.h"
#include "Camera/CameraComponent.h"

// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	cooldDownStartGame = CreateDefaultSubobject<UCoolDownComponent>(TEXT("CoolDown Timer Component"));
	
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();

	controller =  GetWorld()->GetFirstPlayerController();
	gameHUD = Cast<AGameHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	StartGame();
}

void AGameManager::StartMainMenu()
{
	firstStartDone = true;
	if(gameHUD->mainMenuClass)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);

		gameHUD->mainMenu = Cast<UMainMenu>(CreateWidget<UUserWidget>(GetWorld(), gameHUD->mainMenuClass));
		if(gameHUD->mainMenu)
			gameHUD->mainMenu->AddToViewport(99);

		if(controller)
			controller->SetShowMouseCursor(true);
	}
}

void AGameManager::StartGameTimer()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	if(gameHUD->countDownClass)
	{
		gameHUD->countDown = Cast<UCountDownScreen>(CreateWidget<UUserWidget>(GetWorld(), gameHUD->countDownClass));
		if(gameHUD->countDown)
			gameHUD->countDown->AddToViewport();

		gameHUD->countDown->OnStart(cooldDownStartGame);
	}
}

void AGameManager::RemoveEnemy(AEnemy* enemy)
{
	currentEnemies.Remove(enemy);
	enemyCount--;
	
	if (enemyCount <= 0)
	{
		//Win Screen
		FTimerHandle _;
		GetWorldTimerManager().SetTimer(_, this, &AGameManager::WinScreen, 3, false);
	}

}

void AGameManager::StartGame()
{
	if(!firstStartDone)
	{
		StartMainMenu();
		return;
	}
	
	playerRef = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	playerRef->SetActorLocation(FVector(0, 0, 360));
	playerRef->SetActorRotation(FRotator(0, 90, 0));
	playerRef->SetHidden(true);
	
	if(enemyMax % 15 == 0)
		playerRef->healthManager->Heal(playerRef->healthManager->maxLife);
	else
		playerRef->healthManager->Heal();
	
	FVector playerEnemy = FVector(0, startingDistance, 0);
	float angleOffset = 360 / enemyMax;
	FRotator rot(0, angleOffset, 0);

	/* Verification to avoid crash (after 50 enemies max the game crash)*/
	if(enemyMax > 50)
		enemyMax = 50;

	for (int i = 0; i < enemyMax; i++)
	{
		playerEnemy = rot.RotateVector(playerEnemy);
		AEnemy* newEnemy =  GetWorld()->SpawnActor<AEnemy>(enemyBP, playerRef->GetActorLocation() + playerEnemy, FRotator());
		if(newEnemy)
		{
			newEnemy->SpawnDefaultController();
			newEnemy->hitDelegate.BindUObject(soundManager, &ASoundManager::Hit);
			newEnemy->shockDelegate.BindUObject(soundManager, &ASoundManager::Shock);
			currentEnemies.Add(newEnemy);
		}
		else
		{
			DebugError("New enemy couldn't be spawn index : %d", i);
		}
	}
	
	controller->SetShowMouseCursor(false);
	
	enemyCount = enemyMax;
	
	if(gameHUD->victoryScreen && gameHUD->victoryScreen->IsInViewport())
		gameHUD->victoryScreen->RemoveFromViewport();
	else if(gameHUD->mainMenu && gameHUD->mainMenu->IsInViewport())
		gameHUD->mainMenu->RemoveFromViewport();

	StartGameTimer();
}

void AGameManager::WinScreen()
{

	UGameplayStatics::SetGamePaused(GetWorld(), true);
	if(gameHUD->victoryScreenClass)
	{
		gameHUD->victoryScreen = Cast<UVictoryScreen>(CreateWidget<UUserWidget>(GetWorld(), gameHUD->victoryScreenClass));
		if(gameHUD->victoryScreen)
			gameHUD->victoryScreen->AddToViewport();

		if(controller)
			controller->SetShowMouseCursor(true);
	}
}

