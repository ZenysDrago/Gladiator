// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIDirector.h"
#include "GameFramework/Actor.h"
#include "Hammer.generated.h"

class APlayerCharacter;

UCLASS()
class GLADIATORGAME_API AHammer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHammer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hammer")
	TArray<USkeletalMeshComponent*> hammerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hammer")
	class USphereComponent* hammerHitbox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hammer")
	float rotationSpeed;

	UFUNCTION()
	virtual void OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UFUNCTION()
	void SetVelocity(FVector velocity);

	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Effects")
	UNiagaraSystem* niagaraHitLightning;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	APlayerCharacter* playerRef;

	UFUNCTION(BlueprintImplementableEvent)
	void OnDestroy();
	
	bool hitEnemy;
	bool hitWall;

	class UProjectileMovementComponent* projectileMovement;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
