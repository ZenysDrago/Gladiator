// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameManager::StaticClass(), gameManagerList);
	gameManager = Cast<AGameManager>(gameManagerList[0]);
	
	Start->OnClicked.AddDynamic(this, &UMainMenu::OnStart);
	Quit->OnClicked.AddDynamic(this, &UMainMenu::OnQuit);
}

void UMainMenu::OnQuit()
{
	UKismetSystemLibrary::QuitGame(GetWorld() , GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit , true);
}

void UMainMenu::OnStart()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	gameManager->StartGame();
	
}
