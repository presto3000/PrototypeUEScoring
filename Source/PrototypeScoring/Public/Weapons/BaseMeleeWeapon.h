// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Weapons/BaseWeapon.h"
#include "BaseMeleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPESCORING_API ABaseMeleeWeapon : public ABaseWeapon
{
	GENERATED_BODY()

	ABaseMeleeWeapon();

public:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* MeleeCollision;

	UFUNCTION()
	virtual void OnMeleeCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
