// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoolDownComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GLADIATORGAME_API UCoolDownComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCoolDownComponent();

	UPROPERTY(EditAnywhere, Category = "Properties")
	float duration = 0.f;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float speed = 1.f;

	UFUNCTION()
	float GetTime();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Properties")
	float currentTime;
	
	UPROPERTY(VisibleAnywhere , Category = "Properties")
	bool isStarted;

	float savedDuration;

	bool firstStarted;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void CallTick(float DeltaTime);
	
	void ResetTimer();
	
	void Start();
	void Start(const float newDuration);
	
	bool IsOver(bool falseIfNotStarted = false);
};
