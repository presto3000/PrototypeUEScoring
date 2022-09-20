// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractWithCrosshairs.h"
#include "BaseLevelActor.generated.h"

class AMainPlayerController;
UCLASS()
class PROTOTYPESCORING_API ABaseLevelActor : public AActor, public IInteractWithCrosshairs
{
	GENERATED_BODY()
	
public:	
	ABaseLevelActor();
	UPROPERTY()
	AMainPlayerController* PC;

#pragma region Points
    	
	/** Current Actor's Health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Points)
	float Health = 100.f;
	
	/** Max Actor's Health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Points)
	float HP = 100.f;
	
	/** Max amount of points when destroying this */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Points)
	int32 POINTS = 100;
	
	/** Max amount of Style points when destroying this */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Points)
	int32 STYLE_POINTS = 500;

#pragma endregion Points

	
	/** Particles to spawn when hit by bullets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ImpactParticles;

	/** Sound to play when hit by bullets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class USoundCue* ImpactSound;

	virtual void Damage_Implementation(FHitResult HitResult) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(BlueprintNativeEvent)
	void Death();
	virtual void Destroyed() override;
	
	UFUNCTION(BlueprintNativeEvent)
	void ShowHealthBar();
	void ShowHealthBar_Implementation();
	/** In BP */
	UFUNCTION(BlueprintImplementableEvent)
	void HideHealthBar();
	/** Time to display health bar once shot */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float HealthBarDisplayTime = 4.f;
	FTimerHandle HealthBarTimer;

protected:
	virtual void BeginPlay() override;
private:
	// ===================== Components ========================
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ActorStaticMesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* CollisionBox;
	UPROPERTY(EditAnywhere)
	USphereComponent* CollisionSphere;
	
	//===========================================================
	UPROPERTY(EditAnywhere)
	USoundCue* DestroyedSound;
	UPROPERTY(EditAnywhere)
	UParticleSystem* ExplodeParticles;
public:	
	virtual void Tick(float DeltaTime) override;

};
