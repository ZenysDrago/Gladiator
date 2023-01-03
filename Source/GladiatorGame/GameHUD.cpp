// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUD.h"

AGameHUD::AGameHUD()
{
	
}

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();

	if(healthBarClass)
	{
		healthBar = CreateWidget<UHealthBar>(GetWorld(), healthBarClass);
		if(healthBar)
			healthBar->AddToViewport();
	}
	
}

void AGameHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	
}

void AGameHUD::DrawHUD()
{
	Super::DrawHUD();

	
}
