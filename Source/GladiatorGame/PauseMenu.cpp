// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"
#include "PlayerCharacter.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"

void UPauseMenu::NativeConstruct()
{
	Super::NativeConstruct();

	player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	player->scoreDelegate.BindUObject(this, &UPauseMenu::UpdateScoreText);
	UpdateScoreText();

	Resume->OnClicked.AddDynamic(this, &UPauseMenu::OnResume);
	Quit->OnClicked.AddDynamic(this, &UPauseMenu::OnQuit);

}

void UPauseMenu::OnQuit()
{
	UKismetSystemLibrary::QuitGame(GetWorld() , GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit , true);
}

void UPauseMenu::OnResume()
{
	player->StartPauseMenu();
}

void UPauseMenu::UpdateScoreText()
{
	if(TextScore)
		TextScore->SetText(FText::AsNumber(player->score));
}
