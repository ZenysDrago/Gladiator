// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "PlayerCharacter.h"
#include "Entity.h"

// Sets default values
ASoundManager::ASoundManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASoundManager::BeginPlay()
{
	Super::BeginPlay();
	APlayerCharacter* player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	player->parryDelegate.BindUObject(this, &ASoundManager::Parry);
	player->hitDelegate.BindUObject(this, &ASoundManager::Hit);
	player->catchDelegate.BindUObject(this, &ASoundManager::Catch);
}

void ASoundManager::Parry()
{
	UGameplayStatics::PlaySound2D(GetWorld(), parryCue);
}

void ASoundManager::Hit()
{
	UGameplayStatics::PlaySound2D(GetWorld(), hitCue);
}

void ASoundManager::Catch()
{
	UGameplayStatics::PlaySound2D(GetWorld(), catchCue);
}

void ASoundManager::Shock()
{
	UGameplayStatics::PlaySound2D(GetWorld(), shockCue);
}

