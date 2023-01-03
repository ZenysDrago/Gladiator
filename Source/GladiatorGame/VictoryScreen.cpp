// Fill out your copyright notice in the Description page of Project Settings.


#include "VictoryScreen.h"

#include "Enemy.h"
#include "PlayerCharacter.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameManager.h"

void UVictoryScreen::NativeConstruct()
{
	Super::NativeConstruct();

	player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	player->scoreDelegate.BindUObject(this, &UVictoryScreen::UpdateScoreText);
	UpdateScoreText();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameManager::StaticClass(), gameManagerList);
	gameManager = Cast<AGameManager>(gameManagerList[0]);
	
	Continue->OnClicked.AddDynamic(this, &UVictoryScreen::OnContinue);
	Quit->OnClicked.AddDynamic(this, &UVictoryScreen::OnQuit);
}

void UVictoryScreen::OnQuit()
{
	UKismetSystemLibrary::QuitGame(GetWorld() , GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit , true);
}

void UVictoryScreen::OnContinue()
{
	if(GetWorld() && gameManager)
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), enemies);
		for(AActor* enemy : enemies)	
			enemy->Destroy();
		
		gameManager->enemyMax += 2 ;
		gameManager->StartGame();

	}
}

void UVictoryScreen::UpdateScoreText()
{
	if(TextScore)
		TextScore->SetText(FText::AsNumber(player->score));
}
