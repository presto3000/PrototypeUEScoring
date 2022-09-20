// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerComponents/CombatComponent.h"
#include "Weapons/BaseWeapon.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (CombatComponent)
	{
		CombatComponent->Character = this;
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	SpawnDefaultWeapon();
	SpawnSecondaryWeapon();
}

void APlayerCharacter::SpawnDefaultWeapon() const
{
	UWorld* World = GetWorld();
	if (World && IsValid(DefaultWeaponClass))
	{
		ABaseWeapon* StartingWeapon = World->SpawnActor<ABaseWeapon>(DefaultWeaponClass);
		if (CombatComponent)
		{
			CombatComponent->EquipWeapon(StartingWeapon);
		}
	}
}

void APlayerCharacter::SpawnSecondaryWeapon() const
{
	UWorld* World = GetWorld();
	if (World && IsValid(SecondaryWeaponClass))
	{
		ABaseWeapon* SecondaryWeapon = World->SpawnActor<ABaseWeapon>(SecondaryWeaponClass);
		if (CombatComponent)
		{
			CombatComponent->EquipWeapon(SecondaryWeapon);
		}
	}
}

