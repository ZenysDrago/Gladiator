// Fill out your copyright notice in the Description page of Project Settings.


#include "CoolDownComponent.h"
#include "../Debug.h"

// Sets default values for this component's properties
UCoolDownComponent::UCoolDownComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	currentTime = 0.f;
	isStarted = false;
	savedDuration = 0;
	firstStarted = false;
}

float UCoolDownComponent::GetTime()
{
	return currentTime;
}

// Called when the game starts
void UCoolDownComponent::BeginPlay()
{
	Super::BeginPlay();
	savedDuration = duration;
	// ...
	
}


// Called every frame
void UCoolDownComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(isStarted)
		currentTime += (DeltaTime * speed);
}

void UCoolDownComponent::CallTick(float DeltaTime)
{
	if(isStarted)
		currentTime += (DeltaTime * speed);
}

void UCoolDownComponent::ResetTimer()
{
	currentTime = 0;
	isStarted = false;
	firstStarted = true;
}

void UCoolDownComponent::Start()
{
	if(!isStarted)
	{
		firstStarted = true;
		isStarted = true;
		currentTime = 0.f;
	}
}

void UCoolDownComponent::Start(const float newDuration)
{
	savedDuration = duration;
	duration = newDuration;
	Start();
}

bool UCoolDownComponent::IsOver(bool falseIfNotStarted)
{
	if(falseIfNotStarted)
	{
		if(!firstStarted)
			return false;
	}
	else
	{
		if(!firstStarted)
			return true;
	}
	
	if(currentTime >= duration)
	{
		currentTime = 0.f;
		return true;
	}

	return false;
}



