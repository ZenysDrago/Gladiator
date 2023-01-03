// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerCharacter.h"
#include "Debug.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Enemy.h"
#include "GameManager.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include "HealthBar.h"
#include "AIDirector.h"
#include "GameHUD.h"
#include "Hammer.h"
#include "PauseMenu.h"
#include "Materials/MaterialInstanceDynamic.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    characterMovement = GetCharacterMovement();
    characterMovement->bOrientRotationToMovement = true;
    characterMovement->JumpZVelocity = 600.f;
    characterMovement->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    playerController = GetController();
    
    isBlocking = false;
    targetLocked = false;

    inHand = true;
    canThrow = true;

    // Create a camera boom (pulls in towards the player if there is a collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
    CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
    FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    SetupStimulus();

    // LOCK TARGET SETTINGS
    playerSmoothRotSpeed = 10;
    cameraSmoothRotSpeed = 5;
    cameraLockMinPitch = -30;
    cameraLockMaxPitch = -20;
    camPitchMinDistance = 100;
    camPitchMaxDistance = 1500;

    score = 0;

    goldenMatScalar = 0;
    goldenFadeSpeed = 4;

    throwCooldown = 0.5f;

    timerDeath = CreateDefaultSubobject<UCoolDownComponent>(TEXT("Cooldown for death"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
    SetupCamera();
    
    {
        FInputActionHandlerSignature ActionHandler;
        ActionHandler.BindUFunction(this, TEXT("SwitchTarget"), true);
        BindActionBindings(ActionHandler, TEXT("SwitchTargetRight"));
    }

    {
        FInputActionHandlerSignature ActionHandler;
        ActionHandler.BindUFunction(this, TEXT("SwitchTarget"), false);
        BindActionBindings(ActionHandler, TEXT("SwitchTargetLeft"));
    }

    gameHUD = Cast<AGameHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

    auto mat = shieldMesh->GetMaterial(0);
    if (mat) {
        shieldMaterial = Cast<UMaterialInstanceDynamic>(mat->GetMaterial());
        if (!shieldMaterial) {
            shieldMaterial = shieldMesh->CreateAndSetMaterialInstanceDynamic(0);
        }
    }
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (goldenMatScalar > 0)
    {
        goldenMatScalar -= DeltaTime * goldenFadeSpeed;
        shieldMaterial->SetScalarParameterValue("GoldenScalar", goldenMatScalar);
    }

    if (targetLocked)
    {
        //Vector Player Enemy
        FVector forwardLook = enemyLocked->GetActorLocation() - GetActorLocation();
        //Lerp Player Rotation
        FRotator targetRotation = UKismetMathLibrary::MakeRotFromXZ(forwardLook, FVector::UpVector);

        if (!isBlocking)
        {
            FRotator newRotation = FMath::RInterpTo(GetActorRotation(), targetRotation, GetWorld()->GetDeltaSeconds(), playerSmoothRotSpeed);
            SetActorRotation(newRotation);
        }
        
        //Caculate Target Camera Pitch
        float distance = forwardLook.Size();
        float camPitch;
        if (distance < camPitchMinDistance)
            camPitch = cameraLockMinPitch;
        else if(distance > camPitchMaxDistance)
            camPitch = cameraLockMaxPitch;
        else
            camPitch = distance * (cameraLockMaxPitch - cameraLockMinPitch)/(camPitchMaxDistance - camPitchMinDistance) + cameraLockMinPitch;
        
        APlayerController* controller = (APlayerController*)GetController();

        //Lerp Camera Rotation
        FRotator targetCamRot = FRotator(camPitch, targetRotation.Yaw, 0);
        FRotator newCamRot = controller->GetControlRotation();
        newCamRot = FMath::RInterpTo(newCamRot, targetCamRot, GetWorld()->GetDeltaSeconds(), cameraSmoothRotSpeed);
        controller->SetControlRotation(newCamRot);   
    }

    if(timerDeath->IsOver(true))
    {
        APlayerController* controller =  GetWorld()->GetFirstPlayerController();

        isBlocking = false;
        
        UGameplayStatics::SetGamePaused(GetWorld(), true);
        if(gameHUD->gameOverScreenClass)
        {
            gameHUD->gameOverScreen = Cast<UGameOverScreen>(CreateWidget<UUserWidget>(GetWorld(), gameHUD->gameOverScreenClass));
            if(gameHUD->gameOverScreen)
                gameHUD->gameOverScreen->AddToViewport();

            if(controller)
                controller->SetShowMouseCursor(true);
        }
        else
        {
            DebugError("No Widget for Game Over");
        }
    }
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    // Set up gameplay key bindings
    check(PlayerInputComponent);
    //PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    //PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

    // We have 2 versions of the rotation bindings to handle different kinds of devices differently
    // "turn" handles devices that provide an absolute delta, such as a mouse.
    // "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick

    PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);
    //PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);
    //PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);

    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::Attack);
    PlayerInputComponent->BindAction("Block", IE_Pressed, this, &APlayerCharacter::Block);
    PlayerInputComponent->BindAction("Block", IE_Released, this, &APlayerCharacter::UnBlock);

    PlayerInputComponent->BindAction("LockTarget", IE_Pressed, this, &APlayerCharacter::LockEvent);

    PlayerInputComponent->BindAction("HammerThrow", IE_Pressed, this, &APlayerCharacter::ThrowHammer);

    PlayerInputComponent->BindAction("Pause",IE_Pressed, this, &APlayerCharacter::StartPauseMenu).bExecuteWhenPaused = true;
}

void APlayerCharacter::BindActionBindings(FInputActionHandlerSignature& Handler, FName ActionName)
{
    FInputActionBinding ActionBinding = FInputActionBinding(ActionName, IE_Pressed);
    ActionBinding.ActionDelegate = Handler;

    InputComponent->AddActionBinding(ActionBinding);
}

void APlayerCharacter::MoveForward(float value)
{
    if (Controller != nullptr && value != 0.0f && !isBlocking)
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, value);
    }
}

void APlayerCharacter::MoveRight(float value)
{
    if (Controller != nullptr && value != 0.0f && !isBlocking)
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, value);
    }
    
}

//void APlayerCharacter::TurnAtRate(float value)
//{
//    AddControllerYawInput(value * 45.0f * GetWorld()->GetDeltaSeconds());
//}
//
//void APlayerCharacter::LookUpAtRate(float value)
//{
//    AddControllerPitchInput(value * 45.0f * GetWorld()->GetDeltaSeconds());
//
//}

void APlayerCharacter::Turn(float value)
{
    if(!targetLocked)
        AddControllerYawInput(value * 45.0f * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUp(float value)
{
    if (!targetLocked)
        AddControllerPitchInput(value * 45.0f * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::SetupCamera()
{
    if (UWorld* World = GetWorld())
    {
        APlayerCameraManager* cameraManager = World->GetFirstPlayerController()->PlayerCameraManager;

        if (cameraManager != nullptr)
            cameraManager->ViewPitchMax = 45;
        else
            DebugError("No camera Manager");
    }

}

void APlayerCharacter::SetupStimulus()
{
    stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimulusSource"));
    stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
    stimulus->RegisterWithPerceptionSystem();
}

void APlayerCharacter::Attack()
{
    Super::Attack();
    
    if (attackMontage && !isBlocking && inHand && !healthManager->isDead)
    {
        DisableInput((APlayerController*)playerController);
        PlayAnimMontage(attackMontage);

        float duration = GetMesh()->AnimScriptInstance->Montage_Play(attackMontage) * 0.5f;
        FTimerHandle _;
        GetWorldTimerManager().SetTimer(_, this, &APlayerCharacter::OnEndAttack, duration, false);
    }
}

void APlayerCharacter::OnEndAttack()
{
    Super::OnEndAttack();
    
    EnableInput((APlayerController*)playerController);
}

void APlayerCharacter::Block()
{
    isBlocking = true;
}

void APlayerCharacter::UnBlock()
{
    isBlocking = false;
}

void APlayerCharacter::LockEvent()
{
    //No enemies -> Skip
    if (gameManager->currentEnemies.Num() <= 0)
        return;

    //Unlock
    if (UnlockTarget())
        return;

    //Lock
    LockClosestTarget();
}

bool APlayerCharacter::LockClosestTarget()
{
    bool canLock = false;
    float distance = 999999;

    // Find closest enemy 
    for (auto enemy : gameManager->currentEnemies)
    {
        FVector playerEnemy = enemy->GetActorLocation() - GetActorLocation();
        float distancePlayerEnemy = playerEnemy.Size();
        if (distancePlayerEnemy < distance)
        {
            enemyLocked = enemy;
            canLock = true;
            distance = distancePlayerEnemy;
        }
    }
    //Lock
    if (canLock)
    {
        enemyLocked->healthWidget->SetVisibility(true);
        characterMovement->bOrientRotationToMovement = false;
        targetLocked = true;
        //Debug("LOCK");
    }
    //Unlock
    else
    {
        UnlockTarget();
    }

    return canLock;
}

bool APlayerCharacter::UnlockTarget()
{
    if (targetLocked)
    {
        enemyLocked->healthWidget->SetVisibility(false);
        characterMovement->bOrientRotationToMovement = true;
        targetLocked = false;
        //Debug("UNLOCK");
        return true;
    }
    return false;
}

void APlayerCharacter::ThrowHammer()
{   
    if (throwMontage && inHand && canThrow && !isBlocking)
    {
        inHand = false;
        canThrow = false;

        DisableInput((APlayerController*)playerController);
        PlayAnimMontage(throwMontage);
    }
}

void APlayerCharacter::SpawnHammer()
{
    EnableInput((APlayerController*)playerController);
    hammerMesh->SetVisibility(false);
    FVector forwardVector = GetActorForwardVector();
    FVector spawnPos = GetActorLocation() + forwardVector * 100;
    AHammer* hammer = GetWorld()->SpawnActor<AHammer>(hammerBP, spawnPos, forwardVector.Rotation());
    hammer->SetVelocity(FVector(forwardVector.X, forwardVector.Y, 0));
}

void APlayerCharacter::CatchHammer()
{
    catchDelegate.ExecuteIfBound();
    inHand = true;
    hammerMesh->SetVisibility(true);

    FTimerHandle _;
    GetWorldTimerManager().SetTimer(_, FTimerDelegate::CreateLambda([this] { canThrow = true; }), throwCooldown, false);
}

void APlayerCharacter::SwitchTarget(bool toTheRight)
{
    if (!targetLocked || gameManager->enemyCount < 2)
        return;

    float scale = toTheRight ? 1 : -1;

    enemyLocked->healthWidget->SetVisibility(false);
    FVector vectorLocked = enemyLocked->GetActorLocation() - GetActorLocation();
    FRotator lockedRotation = vectorLocked.Rotation();
    float yawDiff = scale * 360;

    for (AEnemy* enemy : gameManager->currentEnemies)
    {
        FVector playerEnemy = enemy->GetActorLocation() - GetActorLocation();
        FRotator playerEnemyRot = playerEnemy.Rotation();
        float newYawDiff = playerEnemyRot.Yaw - lockedRotation.Yaw;

        if (newYawDiff == 0)
            continue;

        newYawDiff = scale * newYawDiff < 0 ? scale * 360 + newYawDiff : newYawDiff;

        if (scale * newYawDiff < scale * yawDiff)
        {
            yawDiff = newYawDiff;
            enemyLocked = enemy;
        }
    }
    enemyLocked->healthWidget->SetVisibility(true);
}

void APlayerCharacter::OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
    AEnemy* enemy = Cast<AEnemy>(OtherActor);
    if (enemy)
    {
        enemy->HitEntity();

        //Lock New Closest Enemy
        if (enemy->healthManager->isDead && targetLocked  && enemy == enemyLocked)
        {
            enemyLocked->healthWidget->SetVisibility(false);
            
            LockClosestTarget();
        }
    }
}

void APlayerCharacter::CheckIfDead()
{
    if (healthManager->isDead)
    {
        APlayerController* controller =  GetWorld()->GetFirstPlayerController();
        DisableInput(controller);
        targetLocked = false;
        UActorComponent* hitbox = (UActorComponent*)hammerHitbox;
        hitbox->UnregisterComponent();
        PlayAnimMontage(deathMontage);

        timerDeath->Start();
        
      
    }
}

void APlayerCharacter::StartPauseMenu()
{
    APlayerController* controller =  GetWorld()->GetFirstPlayerController();
    if(!controller)
    {
        DebugError("No controller");
        return;
    }
    UGameplayStatics::SetGamePaused(GetWorld(), !GetWorld()->IsPaused());
    PauseMenu(controller);
}

void APlayerCharacter::PauseMenu(APlayerController* controller)
{
    if(GetWorld()->IsPaused())
    {
        if(gameHUD->pauseMenuClass)
        {

            gameHUD->pauseMenu = Cast<UPauseMenu>(CreateWidget<UUserWidget>(GetWorld(), gameHUD->pauseMenuClass));
            if(gameHUD->pauseMenu)
                gameHUD->pauseMenu->AddToViewport();
            else
            {
                DebugError("No pause menu");
            }
            
            controller->SetShowMouseCursor(true);        
        }
        else
        {
            DebugError("No Widget for Pause Menu");
        }
    }
    else
    {
        if(gameHUD->pauseMenu)
        {
            gameHUD->pauseMenu->RemoveFromViewport();
            controller->SetShowMouseCursor(false);
        }
    }
}

void APlayerCharacter::UpdateScore()
{
    score++;
    scoreDelegate.ExecuteIfBound();
}

void APlayerCharacter::Parry()
{
    goldenMatScalar = 1;
    shieldMaterial->SetScalarParameterValue("GoldenScalar", goldenMatScalar);
    parryDelegate.ExecuteIfBound();
}
