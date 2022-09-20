// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerComponents/CombatComponent.h"

#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/InteractWithCrosshairs.h"
#include "Kismet/GameplayStatics.h"
#include "PrototypeScoring/MainPlayerController.h"
#include "PrototypeScoring/PlayerCharacter.h"
#include "UI/BaseHUD.h"
#include "Weapons/BaseWeapon.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult HitResult;
	// TraceUnderCrosshairs
	TraceUnderCrosshairs(HitResult);
	// Set Crosshairs
	SetHUDCrosshairs();
	
}

void UCombatComponent::EquipWeapon(ABaseWeapon* WeaponToEquip)
{
	if(Character == nullptr || WeaponToEquip == nullptr) return;
	if(CombatState != ECombatState::Unoccupied) return;
	
	if (EquippedWeapon != nullptr && SecondaryWeapon == nullptr)
	{
		EquipSecondaryWeapon(WeaponToEquip);
	}
	else
	{
		EquipPrimaryWeapon(WeaponToEquip);
	}
}

void UCombatComponent::FireButtonPressed(bool bPressed)
{
	bFireButtonPressed = bPressed;

	if(bFireButtonPressed)
	{
		Fire();
	}
}

void UCombatComponent::SwapWeapons()
{
	if (CombatState != ECombatState::Unoccupied) return;
	ABaseWeapon* TempWeapon = EquippedWeapon;
	EquippedWeapon = SecondaryWeapon;
	SecondaryWeapon = TempWeapon;
	
	EquippedWeapon->SetWeaponState(EWeaponState::Equipped);
	AttachActorToRightHand(EquippedWeapon);
	
	SecondaryWeapon->SetWeaponState(EWeaponState::EquippedSecondary);
	AttachActorToRightPelvis(SecondaryWeapon);
}

bool UCombatComponent::CanFire()
{
	if (EquippedWeapon == nullptr) return false;
	if (CombatState == ECombatState::Melee) return false;
	return CombatState == ECombatState::Unoccupied;
}

void UCombatComponent::Fire()
{
	if(CanFire())
	{
		if (EquippedWeapon && Character)
		{
			//==============================================================================
			// Use HitTarget From TraceUnderCrosshairs or Update it with TraceEndWithScatter
			//==============================================================================
			HitTarget = EquippedWeapon->bUseScatter ? EquippedWeapon->TraceEndWithScatter(HitTarget) : HitTarget;
			if(EquippedWeapon == nullptr) return;

			if(Character && CombatState == ECombatState::Unoccupied)
			{
				EquippedWeapon->Fire(HitTarget);
			}
			// Automatic Fire
			FireTimerStart();
		}
	}
}

void UCombatComponent::EquipPrimaryWeapon(ABaseWeapon* WeaponToEquip)
{
	if (WeaponToEquip == nullptr) return;
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::Equipped);

	AttachActorToRightHand(EquippedWeapon);

	EquippedWeapon->SetOwner(Character);
}

void UCombatComponent::EquipSecondaryWeapon(ABaseWeapon* WeaponToEquip)
{
	if (WeaponToEquip == nullptr) return;
	SecondaryWeapon = WeaponToEquip;
	SecondaryWeapon->SetWeaponState(EWeaponState::EquippedSecondary);
	AttachActorToRightPelvis(WeaponToEquip);

	SecondaryWeapon->SetOwner(Character);
}

void UCombatComponent::AttachActorToRightHand(AActor* ActorToAttach)
{
	if(Character == nullptr || Character->GetMesh() == nullptr || ActorToAttach == nullptr) return;
	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if(HandSocket)
	{
		HandSocket->AttachActor(ActorToAttach, Character->GetMesh());
		if (EquippedWeapon->GetWeaponType() == EWeaponType::Axe)
		{
			EquippedWeapon->SetActorRelativeLocation(FVector(0.f,0.f,5.f));
			EquippedWeapon->SetActorRelativeRotation(FRotator(61.f,0.f,0.f));
		}
	}
}

void UCombatComponent::AttachActorToRightPelvis(AActor* ActorToAttach)
{
	if (Character == nullptr || Character->GetMesh() == nullptr || ActorToAttach == nullptr) return;
	const USkeletalMeshSocket* RightPelvisSocket = Character->GetMesh()->GetSocketByName(FName("RightPelvisSocket"));
	if (RightPelvisSocket)
	{
		RightPelvisSocket->AttachActor(ActorToAttach, Character->GetMesh());
		if (SecondaryWeapon->GetWeaponType() == EWeaponType::AK47)
		{
			SecondaryWeapon->SetActorRelativeLocation(FVector(17.f,0, 0));
			SecondaryWeapon->SetActorRelativeRotation(FRotator(0,0,218.f));
		}
	}
}

void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation (ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	// 2D screen space coordiante into a 3D
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);
	
	if(bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;

		if(Character)
		{
			float DistanceToCharacter = (Character->GetActorLocation() - Start).Size();
			Start += CrosshairWorldDirection * (DistanceToCharacter + 50.f);
			//DrawDebugSphere(GetWorld(), Start, 16.f, 12, FColor::Red, false);
		}
		
		FVector End = Start + CrosshairWorldDirection * TRACE_LENGTH;
		
		GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECollisionChannel::ECC_Visibility);
		
		if(TraceHitResult.GetActor() && TraceHitResult.GetActor()->Implements<UInteractWithCrosshairs>())
		{
			HUDCrosshairStruct.Crosshairscolor = FLinearColor::Red;
		}
		else
		{
			HUDCrosshairStruct.Crosshairscolor = FLinearColor::Green;
		}

		if(!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = End;
			HitTarget = End;
		}
		// when success
		else
		{
			HitTarget = TraceHitResult.ImpactPoint;
			//DrawDebugSphere(GetWorld(), TraceHitResult.ImpactPoint, 12.f, 12.f, FColor::Red,
			//	false, -1);
		}
		//DrawDebugLine(GetWorld(), Start, FVector(Start + HitTarget * TRACE_LENGTH / HitTarget.Size()),
		//FColor::Purple, true);
	}
}

void UCombatComponent::SetHUDCrosshairs()
{
	if(Character == nullptr || Character->Controller == nullptr) return;
	PC = PC == nullptr ? Cast<AMainPlayerController>(Character->Controller) : PC;
	if(PC)
	{
		HUD = HUD == nullptr ? Cast<ABaseHUD>(PC->GetHUD()) : HUD;
		if(HUD)
		{
			if(EquippedWeapon)
			{
				if (EquippedWeapon->CrosshairsCenter == nullptr || EquippedWeapon->CrosshairsLeft == nullptr ||
					EquippedWeapon->CrosshairsRight == nullptr || EquippedWeapon->CrosshairsBottom == nullptr ||
					EquippedWeapon->CrosshairsTop == nullptr) return;
			
				HUDCrosshairStruct.CrosshairsCenter = EquippedWeapon->CrosshairsCenter;
				HUDCrosshairStruct.CrosshairsLeft = EquippedWeapon->CrosshairsLeft;
				HUDCrosshairStruct.CrosshairsRight = EquippedWeapon->CrosshairsRight;
				HUDCrosshairStruct.CrosshairsBottom = EquippedWeapon->CrosshairsBottom;
				HUDCrosshairStruct.CrosshairsTop = EquippedWeapon->CrosshairsTop;
				HUD->SetHUDCrosshairStruct(HUDCrosshairStruct);
			}
			else
			{
				HUDCrosshairStruct.CrosshairsCenter = nullptr;
				HUDCrosshairStruct.CrosshairsLeft = nullptr;
				HUDCrosshairStruct.CrosshairsRight = nullptr;
				HUDCrosshairStruct.CrosshairsBottom = nullptr;
				HUDCrosshairStruct.CrosshairsTop = nullptr;
				HUD->SetHUDCrosshairStruct(HUDCrosshairStruct);
			}
		}
	}
}

void UCombatComponent::FireTimerStart()
{
	if (EquippedWeapon == nullptr || Character == nullptr) return;
	Character->GetWorldTimerManager().SetTimer(FireTimer, this, &UCombatComponent::FireTimerFinished, EquippedWeapon->FireDelay);
}

void UCombatComponent::FireTimerFinished()
{
	if (bFireButtonPressed && EquippedWeapon->bIsAutomatic)
	{
		Fire();
	}
}