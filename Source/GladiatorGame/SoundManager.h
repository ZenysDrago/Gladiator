// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoundManager.generated.h"

UCLASS()
class GLADIATORGAME_API ASoundManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASoundManager();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sounds")
	class USoundCue* hitCue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sounds")
	class USoundCue* parryCue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sounds")
	class USoundCue* catchCue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sounds")
	class USoundCue* shockCue;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION()
	void Parry();

	UFUNCTION()
	void Hit();

	UFUNCTION()
	void Catch();

	UFUNCTION()
	void Shock();
};
