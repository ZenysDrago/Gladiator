// Fill out your copyright notice in the Description page of Project Settings.


#include "Hammer.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Enemy.h"
#include "NiagaraFunctionLibrary.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHammer::AHammer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    hitEnemy = false;
    hitWall = false;

    playerRef = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

    hammerHitbox = CreateDefaultSubobject<USphereComponent>(TEXT("HammerHitbox"));
    hammerHitbox->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    hammerHitbox->SetSphereRadius(40);
    hammerHitbox->SetCollisionProfileName(TEXT("Custom"));
    hammerHitbox->SetCollisionResponseToChannels(ECollisionResponse::ECR_Overlap);
    hammerHitbox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

    for (int i = 1; i < 4; i++)
    {
        FString index = FString::FromInt(i);
        FName name = FName("Hammer" + index);
        hammerMesh.Add(CreateDefaultSubobject<USkeletalMeshComponent>(name));
        hammerMesh.Last()->SetupAttachment(hammerHitbox);
    }

    projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    projectileMovement->InitialSpeed = 1200;
    projectileMovement->ProjectileGravityScale = 0;

    rotationSpeed = 1500.0f;
}

// Called when the game starts or when spawned
void AHammer::BeginPlay()
{
	Super::BeginPlay();
    hammerHitbox->OnComponentBeginOverlap.AddDynamic(this, &AHammer::OnCollision);
}

// Called every frame
void AHammer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    AddActorLocalRotation(FRotator(0.0f, rotationSpeed * DeltaTime, 0.0f));

    if (hitEnemy)
    {
        FVector direction = playerRef->GetActorLocation() - GetActorLocation();
        SetVelocity(FVector(direction));

        if (direction.Size() < 50)
        {
            playerRef->CatchHammer();
            OnDestroy();
            Destroy();
        }
    }
}

void AHammer::OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
    AEnemy* enemy = Cast<AEnemy>(OtherActor);
    if (enemy)
    {
        enemy->HitEntity(true);

        //Lock New Closest Enemy
        if (enemy->healthManager->isDead && playerRef->targetLocked && enemy == playerRef->enemyLocked)
        {
            playerRef->enemyLocked->healthWidget->SetVisibility(false);
            playerRef->LockClosestTarget();
        }
        hammerHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        if(niagaraHitLightning)
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), niagaraHitLightning, enemy->GetActorLocation() + (enemy->GetActorLocation() - playerRef->GetActorLocation()).Normalize()*(-50) );
        }
        
        hitEnemy = true;
    }
    else if(!Cast<APlayerCharacter>(OtherActor))
    {
        hammerHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        hitEnemy = true;
    }
}

void AHammer::SetVelocity(FVector velocity)
{
    /* if no projectile component we destroy the hammer to prevent bug */
    if(projectileMovement)
    {
        velocity.Normalize();
        projectileMovement->Velocity = velocity * projectileMovement->InitialSpeed;     
    }
    else
    {
        playerRef->CatchHammer();
        OnDestroy();
        Destroy();
    }
}

