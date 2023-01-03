// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIEnemyController.h"
#include "GameFramework/Actor.h"
#include "AIDirector.generated.h"

class UCoolDownComponent;

UCLASS()
class GLADIATORGAME_API AAIDirector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAIDirector();
	
	UPROPERTY(EditAnywhere , Category = "Properties")
	AGameManager* gameManager;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void ChooseAttacker();

	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Properties")
	UCoolDownComponent* CoolDownComponent;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
};