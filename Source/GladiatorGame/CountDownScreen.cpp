// Fill out your copyright notice in the Description page of Project Settings.


#include "CountDownScreen.h"

#include <Debug.h>

#include "CoolDownComponent.h"
#include "PlayerCharacter.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UCountDownScreen::UpdateText()
{
	if(!cooldDownGame->IsOver())
	{
		float timeLeft = cooldDownGame->duration - cooldDownGame->GetTime() + 1;
		int timeLeftInt = FMath::RoundToInt(timeLeft);
		countDownText->SetText(FText::AsNumber(timeLeftInt));
	}
	else
	{
		APlayerCharacter* playerRef = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		playerRef->SetHidden(false);

		UGameplayStatics::SetGamePaused(GetWorld(), false);
		this->RemoveFromViewport();
		cooldDownGame->ResetTimer();
	}
}

void UCountDownScreen::OnStart(UCoolDownComponent* cooldDownGameRef)
{
	cooldDownGame = cooldDownGameRef;
	cooldDownGame->ResetTimer();
	cooldDownGame->Start();
}

void UCountDownScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	cooldDownGame->CallTick(InDeltaTime);
	UpdateText();

}

