// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"

#include <string>

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Materials/MaterialInstanceDynamic.h"

void UHealthBar::UpdateLife()
{
	float value = 0;
	
	value = (float)player->healthManager->life / (float)player->healthManager->maxLife;
	value = FMath::Clamp(value, 0.f , 1.f);

	if(!healthBar)
	{
		DebugError("No health bar");
		return;
	}
	
	UMaterialInstanceDynamic* material = healthBar->GetDynamicMaterial();
	if(material){
		material->SetScalarParameterValue(FName("CompleteRange"), value);
	}
	else{
		DebugError("No material");
	}

	if(hpText)
		hpText->SetText(FText::AsNumber(player->healthManager->life));
	else
		Debug("no text");
}

void UHealthBar::NativeConstruct()
{
	Super::NativeConstruct();

	player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	player->healthManager->updatedLifeDelegate.BindUObject(this, &UHealthBar::UpdateLife);
	UpdateLife();
}
