// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthManager.generated.h"

class UHealthBar;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class GLADIATORGAME_API UHealthManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthManager();

	UPROPERTY(EditDefaultsOnly)
	int life = 0;
	UPROPERTY(EditDefaultsOnly)
	int maxLife = 5;

	UPROPERTY(EditDefaultsOnly)
	bool isDead = false;

	UFUNCTION()
	void TakeDamage(int damage = 1.f);

	UFUNCTION()
	void Heal(int healAmount = 1.f);


	/* Delegate called when life is updated */
	DECLARE_DELEGATE(FUpdateLife);
	FUpdateLife updatedLifeDelegate;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
};

