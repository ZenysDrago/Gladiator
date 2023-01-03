  // Fill out your copyright notice in the Description page of Project Settings.


#include "Entity.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "NiagaraFunctionLibrary.h"


// Sets default values
AEntity::AEntity()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    invincibility = CreateDefaultSubobject<UCoolDownComponent>(TEXT("CoolDown Invincibility Component"));
    
	healthManager = CreateDefaultSubobject<UHealthManager>(TEXT("HealthManager"));

    hammerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hammer"));
    hammerMesh->SetupAttachment(GetMesh(), "WeaponPoint");
    hammerMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -10.0f));

    shieldMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shield"));
    shieldMesh->SetupAttachment(GetMesh(), "DualWeaponPoint");

    hammerHitbox = CreateDefaultSubobject<USphereComponent>(TEXT("HammerHitbox"));
    hammerHitbox->SetupAttachment(hammerMesh);
    hammerHitbox->SetRelativeLocation(FVector(0.0f, 60.0f, 20.0f));
    hammerHitbox->SetCollisionProfileName(TEXT("Trigger"));

    redMatScalar = 0;
    redFadeSpeed = 3;
}

// Called when the game starts or when spawned
void AEntity::BeginPlay()
{
	Super::BeginPlay();
    hammerHitbox->OnComponentBeginOverlap.AddDynamic(this, &AEntity::OnCollision);
    hammerHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    gameManager = Cast<AGameManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass()));

    auto mat = GetMesh()->GetMaterial(0);
    if (mat) {
        material = Cast<UMaterialInstanceDynamic>(mat->GetMaterial());
        if (!material) {
            material = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
        }
    }
}

void AEntity::Attack()
{
}

void AEntity::OnEndAttack()
{
}

// Called every frame
void AEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (redMatScalar > 0)
    {
        redMatScalar -= DeltaTime * redFadeSpeed;
        material->SetScalarParameterValue("RedScalar", redMatScalar);
    }
}

// Called to bind functionality to input
void AEntity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEntity::EnableHitbox()
{
    hammerHitbox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AEntity::DisableHitbox()
{
    hammerHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEntity::OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
    
}

void AEntity::CheckIfDead()
{
}

void AEntity::HitEntity(bool shock)
{
    if(invincibility->IsOver())
    {
        if (shock)
        {
            shockDelegate.ExecuteIfBound();
        }

        hitDelegate.ExecuteIfBound();
        healthManager->TakeDamage();
        invincibility->Start();

        if(niagaraHit)
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), niagaraHit, this->GetActorLocation() + FVector(0,0,35));
        }
        
        redMatScalar = 1;
        material->SetScalarParameterValue("RedScalar", redMatScalar);

        CheckIfDead();
    }
}


