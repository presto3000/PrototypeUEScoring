// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PrototypeScoring/Definitions.h"
#include "BaseWeapon.generated.h"

class ABaseLevelActor;
class USoundCue;
class AMainPlayerController;
class APlayerCharacter;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	Initial UMETA(DisplayName = "Inital State"),
	Equipped UMETA(DisplayName = "Equipped State"),
	EquippedSecondary UMETA(DisplayName = "Equipped Secondary"),
};

UCLASS()
class PROTOTYPESCORING_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseWeapon();
	
	UPROPERTY(BlueprintReadWrite, Category = HandyRefs)
	APlayerCharacter* Character;
	UPROPERTY(BlueprintReadWrite, Category = HandyRefs)
	AMainPlayerController* PC;

	void Fire(const FVector& HitTarget);
	void SetWeaponState(EWeaponState State);
	/************************************************
				Counting Points
	*************************************************/
	void CountPointsForTheHit(const ABaseLevelActor* OtherActor);
	
	/************************************************
				Weapon Scatter
	*************************************************/
	FVector TraceEndWithScatter(const FVector& HitTarget);
	UPROPERTY(EditAnywhere, Category = WeaponScatter)
	float DistanceToSphere = 800.f;
	UPROPERTY(EditAnywhere, Category = WeaponScatter)
	float SphereRadius = 75.f;
	UPROPERTY(EditAnywhere, Category = WeaponScatter)
	bool bUseScatter = false;

	/************************************************
					 Crosshair
	*************************************************/
	// Crosshair : Textures for the weapon crosshairs
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsCenter;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsLeft;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsRight;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsBottom;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsTop;
	
	/************************************************
					 Fire Effects
	*************************************************/
	UPROPERTY(EditAnywhere)
	UParticleSystem* BeamParticles;
	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere)
	USoundCue* FireSound;
	UPROPERTY(EditAnywhere, Category = WeaponProperties)
	UAnimationAsset* FireAnimation;
	/************************************************
					 Automatic fire
	*************************************************/
	UPROPERTY(EditAnywhere, Category = AutomaticFire)
	float FireDelay = 0.2;
	UPROPERTY(EditAnywhere, Category = AutomaticFire)
	bool bIsAutomatic = false;
protected:
	virtual void BeginPlay() override;
	void WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit);
	
	UPROPERTY(EditAnywhere)
	USceneComponent* RootSceneComponent;
	UPROPERTY(VisibleAnywhere, Category = WeaponProperties)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere)
	float Damage = 10.f;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EWeaponType WeaponType;
	UPROPERTY(VisibleAnywhere, Category = WeaponProperties)
	EWeaponState WeaponState;
	/************************************************
		Fire Effects on Impact with other surfaces
	*************************************************/
	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactParticles;
	UPROPERTY(EditAnywhere)
	USoundCue* HitSound;


public:	
	virtual void Tick(float DeltaTime) override;
	// Getters:
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const {return WeaponMesh; }
	FORCEINLINE EWeaponType GetWeaponType() const {return WeaponType; }
	FORCEINLINE float GetDamage() const {return Damage; }
};
