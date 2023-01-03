// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthManager.h"
#include "HealthBar.h"

// Sets default values for this component's properties
UHealthManager::UHealthManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UHealthManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	life = maxLife;
}


// Called every frame
void UHealthManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthManager::TakeDamage(int damage)
{
	life -= damage;
	if (life <= 0)
		isDead = true;

	updatedLifeDelegate.ExecuteIfBound();
}

void UHealthManager::Heal(int healAmount)
{
	life += healAmount;
	if (life > maxLife)
		life = maxLife;

	updatedLifeDelegate.ExecuteIfBound();
}