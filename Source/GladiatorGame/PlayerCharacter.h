// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Entity.h"
#include "Components/InputComponent.h"

#include "PlayerCharacter.generated.h"

class AEnemy;
class UPauseMenu;
class AHammer;


UCLASS()
class GLADIATORGAME_API APlayerCharacter : public AEntity
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimMontage* throwMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	bool isBlocking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	bool targetLocked;

	bool inHand;

	AController* playerController;
	UCharacterMovementComponent* characterMovement;
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Death")
	UCoolDownComponent* timerDeath;
	
	// LOCK TARGET SETTINGS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock Target")
	float playerSmoothRotSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock Target")
	float cameraSmoothRotSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock Target")
	float cameraLockMinPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock Target")
	float cameraLockMaxPitch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lock Target")
	float camPitchMinDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lock Target")
	float camPitchMaxDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Instanciate")
	TSubclassOf<AHammer> hammerBP;
	

	UPROPERTY(EditAnywhere, Category = "Score")
	int score;

	DECLARE_DELEGATE(UpdateScorePlayer)
	UpdateScorePlayer scoreDelegate;

	UFUNCTION()
	void UpdateScore();

	UFUNCTION()
	void StartPauseMenu();

	UFUNCTION()
	void Parry();

	UFUNCTION(BlueprintCallable)
	void SpawnHammer();

	UFUNCTION(BlueprintCallable)
	void CatchHammer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throw")
	float throwCooldown;

	DECLARE_DELEGATE(ParryEvent)
	ParryEvent parryDelegate;

	DECLARE_DELEGATE(CatchEvent)
	CatchEvent catchDelegate;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void Turn(float value);
	void LookUp(float value);
	//void TurnAtRate(float rate);
	//void LookUpAtRate(float rate);
	

	virtual void Attack() override ;
	virtual void OnEndAttack() override ;

	void Block();
	void UnBlock();

	void LockEvent();
	bool UnlockTarget();

	void ThrowHammer();

	virtual void OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit) override;

	virtual void CheckIfDead() override;
	
	UMaterialInstanceDynamic* shieldMaterial;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void BindActionBindings(FInputActionHandlerSignature& Handler, FName ActionName);

	AEnemy* enemyLocked;
	bool LockClosestTarget();

private:
	void SetupCamera();

	void PauseMenu(APlayerController* controller);
	
	class UAIPerceptionStimuliSourceComponent* stimulus;

	class AGameHUD* gameHUD;


	
	UFUNCTION()
	void SetupStimulus();

	UFUNCTION()
	void SwitchTarget(bool toTheRight);

	float goldenMatScalar;
	float goldenFadeSpeed;

	bool canThrow;
};
