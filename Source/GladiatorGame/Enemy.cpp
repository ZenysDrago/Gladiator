// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"

#include <basetyps.h>
#include <string>

#include "PlayerCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameManager.h"
#include "EnemyHealthBar.h"

#include "AIEnemyController.h"
#include "Animation/AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AEnemy::AEnemy()
{
	coolDownAttack = CreateDefaultSubobject<UCoolDownComponent>(TEXT("CoolDown Attack Component"));
	rangeTolerance = 350;
	UCharacterMovementComponent* charMov = GetCharacterMovement();
	charMov->MaxWalkSpeed = 850;

	healthWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	healthWidget->SetupAttachment(GetCapsuleComponent());
	healthWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));
	healthWidget->SetWidgetSpace(EWidgetSpace::Screen);
	healthWidget->SetDrawAtDesiredSize(true);
	healthWidget->SetVisibility(false);
	isAttacking = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	UEnemyHealthBar* healthBar = Cast<UEnemyHealthBar>(healthWidget->GetUserWidgetObject());
	if(healthBar)
		healthBar->SetOwnerEnemy(this);

	coolDownAttack->Start();
}

void AEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!isAttacking && !healthManager->isDead)
	{
		FVector forwardLook = player->GetActorLocation() - GetActorLocation();
		SetActorRotation(UKismetMathLibrary::MakeRotFromXZ(forwardLook , FVector::UpVector));
	}

	if(coolDownAttack->IsOver())
		isAttacking = false;
}

void AEnemy::OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (player == Cast<APlayerCharacter>(OtherActor))
	{
		FVector2D playerForward = FVector2D(player->GetActorForwardVector().X, player->GetActorForwardVector().Y);
		FVector2D playerEnemy = FVector2D(GetActorForwardVector().X, GetActorForwardVector().Y);
		if (player->isBlocking && FVector2D::DotProduct(playerForward, playerEnemy) < -0.25f)
		{
			player->Parry();
			return;
		}
		player->HitEntity();
	}
}

void AEnemy::CheckIfDead()
{
	if (healthManager->isDead)
	{
		UCapsuleComponent* capsule = GetCapsuleComponent();
		capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UActorComponent* hitbox = (UActorComponent*)hammerHitbox;
		hitbox->UnregisterComponent();
		PlayAnimMontage(deathMontage);
		
		AAIEnemyController* controller = Cast<AAIEnemyController>(GetController());
		controller->behaviorTreeComp->StopTree();
		player->UpdateScore();
		
		gameManager->RemoveEnemy(this);
	}
}


void AEnemy::Attack()
{
	Super::Attack();
	
	if (attackMontage && !healthManager->isDead)
	{
		/* Disable behavior tree */
		AAIEnemyController* controller = Cast<AAIEnemyController>(GetController());
		if(!controller)
		{
			DebugError("No AI Controller ")
			return;
		}
		controller->behaviorTreeComp->StopTree();

		
		PlayAnimMontage(attackMontage);

		float duration = GetMesh()->AnimScriptInstance->Montage_Play(attackMontage);
		FTimerHandle _;
		GetWorldTimerManager().SetTimer(_, this, &AEnemy::OnEndAttack, duration, false);
	}
}

void AEnemy::OnEndAttack()
{
	Super::OnEndAttack();

	AAIEnemyController* controller = Cast<AAIEnemyController>(GetController());
	if(!controller)
	{
		DebugError("No AI Controller ")
		return;
	}
	
	/*Restart the tree*/
	controller->behaviorTreeComp->RestartTree();
	controller->blackboardComp->SetValueAsBool("IsAttacking", false);
	coolDownAttack->Start();
}
