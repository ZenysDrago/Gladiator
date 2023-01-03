// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Entity.h"
#include "Perception/AISightTargetInterface.h"
#include "Components/WidgetComponent.h"

#include "Enemy.generated.h"


class APlayerCharacter;

UCLASS()
class GLADIATORGAME_API AEnemy : public AEntity , public IAISightTargetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();
	virtual void OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit) override;

	virtual void CheckIfDead() override;

	UPROPERTY(EditAnywhere, Category = AI)
	class UBehaviorTree* behaviorTree;

	UPROPERTY(VisibleAnywhere)
	bool isAttacking;

	UPROPERTY(EditAnywhere)
	float rangeTolerance;

	UPROPERTY(EditAnywhere)
	float enemyTolerance;

	UPROPERTY(EditAnywhere)
	float rangeAttack;

	UPROPERTY(EditAnywhere, Category = "AI")
	FName targetBone = "Head";

	UPROPERTY(EditAnywhere, Category = "HealthBar")
	class UWidgetComponent* healthWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "CoolDown")
	UCoolDownComponent* coolDownAttack;
	
	APlayerCharacter* player;

	/* Verification to prevent locking enemies */
	FVector lastPos;
	UPROPERTY(VisibleAnywhere)
	int cptPos = 0;
	
	virtual void Attack() override;
	virtual void OnEndAttack() override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual  void Tick(float DeltaSeconds) override;

};
