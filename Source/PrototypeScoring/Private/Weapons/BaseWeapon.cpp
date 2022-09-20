// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/BaseWeapon.h"

#include "Engine/SkeletalMeshSocket.h"
#include "Interfaces/InteractWithCrosshairs.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "LevelActors/BaseLevelActor.h"
#include "Math/UnitConversion.h"
#include "Particles/ParticleSystemComponent.h"
#include "PrototypeScoring/MainPlayerController.h"
#include "Sound/SoundCue.h"


ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponSceneComponent"));
	SetRootComponent(RootSceneComponent);
	
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootSceneComponent);
	
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetEnableGravity(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseWeapon::Fire(const FVector& HitTarget)
{
	// Fire Animation
	if(FireAnimation)
	{
		WeaponMesh->PlayAnimation(FireAnimation, false);
	}

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr) return;
	
	AController* InstigatorController = OwnerPawn->GetController();

	// Get Socket Transform
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if(MuzzleFlashSocket && InstigatorController)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();


		FHitResult FireHit;

		WeaponTraceHit(Start, HitTarget, FireHit);


		// Does hit Actor implement InteractWithCrosshairs Interface?
		if (IsValid(FireHit.GetActor()))
		{
			// Check if Actor inherits from Interface
			IInteractWithCrosshairs* InteractWithCrosshairsInterface = Cast<IInteractWithCrosshairs>(FireHit.GetActor());
			if (InteractWithCrosshairsInterface)
			{
				InteractWithCrosshairsInterface->Damage_Implementation(FireHit);
				
				ABaseLevelActor* BaseActor = Cast<ABaseLevelActor>(FireHit.GetActor());
				if (BaseActor)
				{
					// Counting Points
					CountPointsForTheHit(BaseActor);
				}

				// Apply Damage
				UGameplayStatics::ApplyDamage(FireHit.GetActor(), Damage, InstigatorController, this, UDamageType::StaticClass());

			}
			else
			{
				// Spawn Particles if FirehitResult is not a valid Actor
				if(ImpactParticles)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, FireHit.ImpactPoint, FireHit.ImpactNormal.Rotation());
				}
				// Play Impact Sound
				if(HitSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, HitSound, FireHit.ImpactPoint);
				}
			}
			

		}
		// Muzzle Flash Emitter
		if(MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
			
		}
		// FireSound
		if(FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}
	}
}
void ABaseWeapon::WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit)
{
	const AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		const FVector End = TraceStart + (HitTarget - TraceStart) * 1.25f;
		const UWorld* World = GetWorld();
		
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
	
		World->LineTraceSingleByChannel(OutHit, TraceStart, End, ECollisionChannel::ECC_Visibility, QueryParams);
	
		FVector BeamEnd = End;
		if(OutHit.bBlockingHit)
		{
			BeamEnd = OutHit.ImpactPoint;
		}
		//DrawDebugSphere(GetWorld(), BeamEnd, 16.f, 12, FColor::Orange, false, 2);
		if(BeamParticles)
		{
			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(World, BeamParticles, TraceStart, FRotator::ZeroRotator);
			if(Beam)
			{
				Beam->SetVectorParameter(FName("Target"), BeamEnd);
			}
		}
		//DrawDebugLine(GetWorld(), TraceStart, FVector(BeamEnd),
		//FColor::Cyan, false, 2.f);
	}
}
void ABaseWeapon::SetWeaponState(EWeaponState State)
{
	WeaponState = State;

}

void ABaseWeapon::CountPointsForTheHit(const ABaseLevelActor* OtherActor)
{
	PC = PC == nullptr ? Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)) : PC;

	// Points Damaged from Max HP
	const float PointsProc = Damage / OtherActor->HP * 100;
	// Update Mulitiplier Value with Timers
	PC->UpdateMultiplierTimer(PC->PlayerStylePoints);
	// Count StylePoints to Add
	const float StylePointsToAdd = PointsProc * OtherActor->STYLE_POINTS / 100;
	// Sum StylePoints for the Player AFTER setting the multiplier (don't want to count before hitting the Constraint Value)
	PC->PlayerStylePoints += StylePointsToAdd;
	// Points to Add
	const float PointsToAdd = PointsProc * OtherActor->POINTS / 100;
	// Score Value = ScoreValue + (PointsToAdd * multiplier) 
	PC->ScoreValue += static_cast<int32>(PointsToAdd) * PC->MultiplierValue;

	PC->SetPointsValue(static_cast<int32>(PointsToAdd));
	PC->SetScoreValue(PC->ScoreValue);
	PC->SetMultiplierValue(PC->MultiplierValue);
	PC->ShowPointsMultiplierHorizontalBox();
		
	PC->PlayHorizontalBoxPointsAnim();
}


FVector ABaseWeapon::TraceEndWithScatter(const FVector& HitTarget)
{
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if(MuzzleFlashSocket == nullptr) return FVector();
	
	const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
	const FVector TraceStart = SocketTransform.GetLocation();


	const FVector ToTargetNormalized = (HitTarget - TraceStart).GetSafeNormal();
	const FVector SphereCenter = TraceStart + ToTargetNormalized * DistanceToSphere;

	const FVector RandVec = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, SphereRadius);
	const FVector EndLoc = SphereCenter + RandVec;
	const FVector ToEndLoc = EndLoc - TraceStart;

	//DrawDebugSphere(GetWorld(), SphereCenter, SphereRadius, 12, FColor::Red, false, 2.f);
	//DrawDebugSphere(GetWorld(), ToEndLoc, 4.f, 12, FColor::Orange, false, 2.f);
	
	//DrawDebugLine(GetWorld(), TraceStart, FVector(TraceStart + ToEndLoc * TRACE_LENGTH / ToEndLoc.Size()),
	//	FColor::Cyan, true);
	
	return FVector(TraceStart + ToEndLoc * TRACE_LENGTH / ToEndLoc.Size());
}
