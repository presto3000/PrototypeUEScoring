// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class ABaseWeapon;
class UCombatComponent;
UCLASS()
class PROTOTYPESCORING_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCombatComponent* CombatComponent;
	
	/************************************************
			Input BP Callbacks
	*************************************************/
	UFUNCTION(BlueprintImplementableEvent)
	void OnAimButtonPressedBP();
	UFUNCTION(BlueprintImplementableEvent)
	void OnAimButtonReleasedBP();
	UFUNCTION(BlueprintImplementableEvent)
	void OnSwapWeaponsBP();
	
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* AttackWeaponMontage;
protected:
	virtual void BeginPlay() override;
	void SpawnDefaultWeapon() const;
	void SpawnSecondaryWeapon() const;
		
	/************************************************
			Default Weapon and 2ndary Weapon
	*************************************************/
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseWeapon> DefaultWeaponClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseWeapon> SecondaryWeaponClass;




};
