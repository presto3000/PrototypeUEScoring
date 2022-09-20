// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BaseMeleeWeapon.h"

#include "Kismet/GameplayStatics.h"
#include "LevelActors/BaseLevelActor.h"
#include "PrototypeScoring/MainPlayerController.h"

ABaseMeleeWeapon::ABaseMeleeWeapon()
{
	MeleeCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeCollision"));
	MeleeCollision->SetupAttachment(WeaponMesh);
}

void ABaseMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (MeleeCollision)
	{
		MeleeCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseMeleeWeapon::OnMeleeCollisionOverlap);
	}
}

void ABaseMeleeWeapon::OnMeleeCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABaseLevelActor* BaseActor = Cast<ABaseLevelActor>(OtherActor))
	{
		CountPointsForTheHit(BaseActor);
		
		// Apply Damage
		UGameplayStatics::ApplyDamage(BaseActor, Damage, PC, this, UDamageType::StaticClass());

		BaseActor->ShowHealthBar();
		BaseActor->ShowHealthBar_Implementation();
	}
}
