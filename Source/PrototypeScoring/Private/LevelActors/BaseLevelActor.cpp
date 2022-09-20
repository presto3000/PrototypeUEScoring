// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelActors/BaseLevelActor.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ABaseLevelActor::ABaseLevelActor()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetRootComponent(CollisionBox);
	
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->SetSphereRadius(150.f);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	
	ActorStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ActorStaticMesh"));
	ActorStaticMesh->SetupAttachment(RootComponent);
}

void ABaseLevelActor::Damage_Implementation(FHitResult HitResult)
{
	if(ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
	if(ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, HitResult.Location, FRotator(0.f), true);
	}
	// Show HealthBar
	ShowHealthBar();
	ShowHealthBar_Implementation();
}

float ABaseLevelActor::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{

	if(Health - DamageAmount <= 0.f)
	{
		Health = 0.f;
		Death();
	}
	else
	{

		Health -= DamageAmount;
	}
	return  DamageAmount;
}

void ABaseLevelActor::Destroyed()
{
	Super::Destroyed();

	if(DestroyedSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DestroyedSound, GetActorLocation());
	}

	if (ExplodeParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplodeParticles, GetActorLocation());
	}
}

void ABaseLevelActor::Death_Implementation()
{
	// Hide HealthBar
	HideHealthBar();
	Destroy();
}

void ABaseLevelActor::ShowHealthBar_Implementation()
{
	GetWorldTimerManager().ClearTimer(HealthBarTimer);
	GetWorldTimerManager().SetTimer(HealthBarTimer,	this, &ABaseLevelActor::HideHealthBar, HealthBarDisplayTime);
}

void ABaseLevelActor::BeginPlay()
{
	Super::BeginPlay();
}


void ABaseLevelActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

