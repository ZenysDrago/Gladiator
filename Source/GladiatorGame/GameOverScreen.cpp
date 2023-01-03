// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverScreen.h"
#include "PlayerCharacter.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


void UGameOverScreen::NativeConstruct()
{
	Super::NativeConstruct();

	player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	player->scoreDelegate.BindUObject(this, &UGameOverScreen::UpdateScoreText);
	UpdateScoreText();

	Restart->OnClicked.AddDynamic(this, &UGameOverScreen::OnRestart);
	Quit->OnClicked.AddDynamic(this, &UGameOverScreen::OnQuit);
}

void UGameOverScreen::OnQuit()
{
	UKismetSystemLibrary::QuitGame(GetWorld() , GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit , true);
}

void UGameOverScreen::OnRestart()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void UGameOverScreen::UpdateScoreText()
{
	if(TextScore)
		TextScore->SetText(FText::AsNumber(player->score));
}
