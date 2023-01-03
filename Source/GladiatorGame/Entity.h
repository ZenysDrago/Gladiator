// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Debug.h"
#include "CoreMinimal.h"
#include "CoolDownComponent.h"
#include "GameFramework/Character.h"
#include "HealthManager.h"

#include "Entity.generated.h"

class UNiagaraSystem;
class AGameManager;

UCLASS()
class GLADIATORGAME_API AEntity : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEntity();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimMontage* attackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimMontage* deathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimBlueprint* playerAnimBP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	UHealthManager* healthManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	USkeletalMeshComponent* hammerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	USkeletalMeshComponent* shieldMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hitbox")
	class USphereComponent* hammerHitbox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "CoolDown")
	UCoolDownComponent* invincibility;
	
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Effects")
	UNiagaraSystem* niagaraHit;
	
	UFUNCTION(BlueprintCallable)
	void EnableHitbox();
	UFUNCTION(BlueprintCallable)
	void DisableHitbox();

	UFUNCTION()
	virtual void OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UFUNCTION()
	virtual void CheckIfDead();

	UFUNCTION()
	void HitEntity(bool shock = false);

	DECLARE_DELEGATE(HitEvent)
	HitEvent hitDelegate;

	DECLARE_DELEGATE(ShockEvent)
	ShockEvent shockDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AGameManager* gameManager;

	virtual void Attack();
	virtual void OnEndAttack();

	UMaterialInstanceDynamic* material;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	float redMatScalar;
	float redFadeSpeed;

};
