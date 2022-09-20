// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PrototypeScoring/Definitions.h"
#include "UI/BaseHUD.h"
#include "CombatComponent.generated.h"


class ABaseHUD;
class AMainPlayerController;
class APlayerCharacter;
class ABaseWeapon;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTOTYPESCORING_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

	void EquipWeapon(ABaseWeapon* WeaponToEquip);
	void FireButtonPressed(bool bPressed);
	UPROPERTY()
	ECombatState CombatState = ECombatState::Unoccupied;

	void SwapWeapons();
	bool bFireButtonPressed = false;
	bool CanFire();
	void Fire();
	void EquipPrimaryWeapon(ABaseWeapon* WeaponToEquip);
	void EquipSecondaryWeapon(ABaseWeapon* WeaponToEquip);
	void AttachActorToRightHand(AActor* ActorToAttach);
	void AttachActorToRightPelvis(AActor* ActorToAttach);


	/************************************************
				 Crosshair
	*************************************************/
	void TraceUnderCrosshairs(FHitResult& TraceHitResult);
	void SetHUDCrosshairs();
	FHUDCrosshairStruct HUDCrosshairStruct;	

	/************************************************
				HandyRefs
	*************************************************/
	UPROPERTY(VisibleAnywhere, Category = HandyRefs)
	APlayerCharacter* Character;
	UPROPERTY(VisibleAnywhere, Category = HandyRefs)
	AMainPlayerController* PC;
	UPROPERTY(VisibleAnywhere, Category = HandyRefs)
	ABaseHUD* HUD;
	
	FVector HitTarget;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ABaseWeapon* EquippedWeapon;
	UPROPERTY()
	ABaseWeapon* SecondaryWeapon;

	/************************************************
				 Automatic fire
	*************************************************/
	FTimerHandle FireTimer;
	
	void FireTimerStart();
	void FireTimerFinished();
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
