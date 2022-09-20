// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

#include "PlayerCharacter.h"
#include "PlayerComponents/CombatComponent.h"
#include "UI/BaseHUD.h"
#include "UI/MainPlayerOverlay.h"
#include "Weapons/BaseWeapon.h"

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Actions
	InputComponent->BindAction("Aim", IE_Pressed, this, &AMainPlayerController::OnAimButtonPressed);
	InputComponent->BindAction("Aim", IE_Released, this, &AMainPlayerController::OnAimButtonReleased);
	InputComponent->BindAction("Fire", IE_Pressed, this, &AMainPlayerController::OnFireButtonPressed);
	InputComponent->BindAction("Fire", IE_Released, this, &AMainPlayerController::OnFireButtonReleased);
	InputComponent->BindAction("SwapWeapons", IE_Pressed, this, &AMainPlayerController::OnSwapWeaponsPressed);
	InputComponent->BindAction("ToggleAutomatic", IE_Pressed, this, &AMainPlayerController::OnToggleAutomaticPressed);
}

void AMainPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	PlayerCharacter = Cast<APlayerCharacter>(InPawn);
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	BaseHUD = BaseHUD == nullptr ? Cast<ABaseHUD>(GetHUD()) : BaseHUD;
	if(BaseHUD)
	{
		if(BaseHUD->MainPlayerOverlayRef == nullptr) BaseHUD->AddCharacterOverlay();
	}
}

void AMainPlayerController::OnAimButtonPressed()
{
	if (PlayerCharacter == nullptr) return;
	
	PlayerCharacter->OnAimButtonPressedBP();
}

void AMainPlayerController::OnAimButtonReleased()
{
	if (PlayerCharacter == nullptr) return;
	
	PlayerCharacter->OnAimButtonReleasedBP();
}

void AMainPlayerController::OnFireButtonPressed()
{
	if (PlayerCharacter == nullptr || PlayerCharacter->CombatComponent == nullptr) return;
	
	
	if (PlayerCharacter->CombatComponent->EquippedWeapon)
	{
		if (PlayerCharacter->CombatComponent->EquippedWeapon->GetWeaponType() == EWeaponType::Axe && PlayerCharacter->GetMesh())
		{
			if (UAnimInstance* AnimInstance = PlayerCharacter->GetMesh()->GetAnimInstance())
			{
				if (!AnimInstance->Montage_IsPlaying(PlayerCharacter->AttackWeaponMontage))
				{
					AnimInstance->Montage_Play(PlayerCharacter->AttackWeaponMontage);
				}
			}
		}
		else
		{
			PlayerCharacter->CombatComponent->FireButtonPressed(true);
			PlayerCharacter->OnAimButtonPressedBP();
		}
	}

}

void AMainPlayerController::OnFireButtonReleased()
{
	if (PlayerCharacter == nullptr || PlayerCharacter->CombatComponent == nullptr) return;

	PlayerCharacter->CombatComponent->FireButtonPressed(false);
}

void AMainPlayerController::OnSwapWeaponsPressed()
{
	if (PlayerCharacter == nullptr || PlayerCharacter->CombatComponent == nullptr) return;

	PlayerCharacter->CombatComponent->SwapWeapons();
	PlayerCharacter->OnSwapWeaponsBP();
}

void AMainPlayerController::OnToggleAutomaticPressed()
{
	if (PlayerCharacter == nullptr || PlayerCharacter->CombatComponent == nullptr) return;
	if (PlayerCharacter->CombatComponent->CanFire() && PlayerCharacter->CombatComponent->EquippedWeapon->bIsAutomatic)
	{
		PlayerCharacter->CombatComponent->EquippedWeapon->bIsAutomatic = false;
	}
	else
	{
		PlayerCharacter->CombatComponent->EquippedWeapon->bIsAutomatic = true;
	}
}

void AMainPlayerController::OnMoveForwardBackwards(float AxisValue)
{
	if (PlayerCharacter)
	{
		if (AxisValue != 0.0f)
		{
			// find out which way is forward
			const FRotator Rotation = this->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			PlayerCharacter->AddMovementInput(Direction, AxisValue);
		}
	}
}

void AMainPlayerController::OnMoveRightLeft(float AxisValue)
{
	if (PlayerCharacter == nullptr) return;
	if(AxisValue != 0.f)
	{
		const FRotator YawRotation(0.f, this->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		PlayerCharacter->AddMovementInput(Direction, AxisValue);
	}
}

void AMainPlayerController::OnLookUpDown(float AxisValue)
{
	if (PlayerCharacter)
	{
		PlayerCharacter->AddControllerPitchInput(AxisValue);
	}
}

void AMainPlayerController::OnLookLeftRight(float AxisValue)
{
	if (PlayerCharacter)
	{
		PlayerCharacter->AddControllerYawInput(AxisValue);
	}
}

void AMainPlayerController::UpdateMultiplierTimer(int32 PlayerStylePoints_)
{
	// Check PlayerStyle Points Constraints

	if (PlayerStylePoints_ < 100)
	{
		MultiplierValue = MultiplierValueArray[0];
		Duration = 0;
	}
	else if (PlayerStylePoints_ >= 100 && PlayerStylePoints_ < 150)
	{
		MultiplierValue = MultiplierValueArray[1];
		Duration = MultiplierTimeValueArray[0];
	}
	else if (PlayerStylePoints_ >= 150 && PlayerStylePoints_ < 200)
	{
		MultiplierValue = MultiplierValueArray[2];
		Duration = MultiplierTimeValueArray[1];
	}
	else if (PlayerStylePoints_ >= 200 && PlayerStylePoints_ < 250)
	{
		MultiplierValue = MultiplierValueArray[3];
		Duration = MultiplierTimeValueArray[2];	
	}
	else if (PlayerStylePoints_ >= 250 && PlayerStylePoints_ < 300)
	{
		MultiplierValue = MultiplierValueArray[4];
		Duration = MultiplierTimeValueArray[3];	
	}
	else if (PlayerStylePoints_ >= 300 && PlayerStylePoints_ < 350)
	{
		MultiplierValue = MultiplierValueArray[5];
		Duration = MultiplierTimeValueArray[4];
	}
	else if (PlayerStylePoints_ >= 350 && PlayerStylePoints_ < 400)
	{
		MultiplierValue = MultiplierValueArray[6];
		Duration = MultiplierTimeValueArray[5];
	}
	else if (PlayerStylePoints_ >= 400 && PlayerStylePoints_ < 450)
	{
		MultiplierValue = MultiplierValueArray[7];
		Duration = MultiplierTimeValueArray[6];
	}
	else if (PlayerStylePoints_ >= 450 && PlayerStylePoints_ < 500)
	{
		MultiplierValue = MultiplierValueArray[8];
		Duration = MultiplierTimeValueArray[7];
	}
	else
	{
		MultiplierValue = MultiplierValueArray[9];
		Duration = MultiplierTimeValueArray[8];
	}

	// Update Multiplier Value in UI
	SetMultiplierValue(MultiplierValue);
	
	if (TimerHandle_PointsMultiplier.IsValid())
	{
		ClearMultiplierTimer();
	}
	;
	if(const UWorld* World = GetWorld())
	{
		// Set Timer
		World->GetTimerManager().SetTimer(TimerHandle_PointsMultiplier, this, &AMainPlayerController::UpdateDurationAndStylePoints, Duration, false);
	}
}

void AMainPlayerController::UpdateDurationAndStylePoints()
{

	if(const UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle_PointsMultiplier);
	}
	int32 DurationInt = static_cast<int32>(Duration);
	// Update by Duration, Style Points when Idle and activate UpdateMultiplierTimer
	switch(DurationInt)
	{
	case 0:
		{}
		break;
	case 15:
		PlayerStylePoints = 0;
		UpdateMultiplierTimer(PlayerStylePoints);
		break;
	case 10:
		PlayerStylePoints = 100;
		UpdateMultiplierTimer(PlayerStylePoints);
		break;
	case 8:
		PlayerStylePoints = 150;
		UpdateMultiplierTimer(PlayerStylePoints);
		break;
	case 7:
		PlayerStylePoints = 200;
		UpdateMultiplierTimer(PlayerStylePoints);
		break;
	case 6:
		PlayerStylePoints = 250;
		UpdateMultiplierTimer(PlayerStylePoints);
		break;
	case 5:
		PlayerStylePoints = 300;
		UpdateMultiplierTimer(PlayerStylePoints);
		break;
	case 4:
		PlayerStylePoints = 350;
		UpdateMultiplierTimer(PlayerStylePoints);
		break;
	case 3:
		PlayerStylePoints = 400;
		UpdateMultiplierTimer(PlayerStylePoints);
		break;
	case 2:
		PlayerStylePoints = 450;
		UpdateMultiplierTimer(PlayerStylePoints);
		break;
	case 1:
		PlayerStylePoints = 500;
		UpdateMultiplierTimer(PlayerStylePoints);
		break;
	default:
		{}
		break;
	}
}

void AMainPlayerController::ClearMultiplierTimer()
{
	if(const UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle_PointsMultiplier);
	}
}

void AMainPlayerController::SetScoreValue(const int32 Score)
{
	BaseHUD = BaseHUD == nullptr ? Cast<ABaseHUD>(GetHUD()) : BaseHUD;
	if (BaseHUD && BaseHUD->MainPlayerOverlayRef && BaseHUD->MainPlayerOverlayRef->TxtScore && BaseHUD->MainPlayerOverlayRef->TxtScore_2)
	{
		const FString ScoreText = FString::Printf(TEXT("%d"), Score);
		BaseHUD->MainPlayerOverlayRef->TxtScore->SetText(FText::FromString(ScoreText));
		BaseHUD->MainPlayerOverlayRef->TxtScore_2->SetText(FText::FromString(ScoreText));
	}
}

void AMainPlayerController::SetPointsValue(const int32 PointsValue)
{
	BaseHUD = BaseHUD == nullptr ? Cast<ABaseHUD>(GetHUD()) : BaseHUD;
	if (BaseHUD && BaseHUD->MainPlayerOverlayRef && BaseHUD->MainPlayerOverlayRef->TxtPoints)
	{
		const FString PointsText = FString::Printf(TEXT("%d"), PointsValue);
		BaseHUD->MainPlayerOverlayRef->TxtPoints->SetText(FText::FromString(PointsText));
	}
}

void AMainPlayerController::SetMultiplierValue(const int32 Multiplier)
{
	BaseHUD = BaseHUD == nullptr ? Cast<ABaseHUD>(GetHUD()) : BaseHUD;
	if (BaseHUD && BaseHUD->MainPlayerOverlayRef && BaseHUD->MainPlayerOverlayRef->TxtStylePointsMultiplier)
	{
		const FString MultiplierValueText = FString::Printf(TEXT("%d"), Multiplier);
		BaseHUD->MainPlayerOverlayRef->TxtStylePointsMultiplier->SetText(FText::FromString(MultiplierValueText));
	}
}

void AMainPlayerController::ShowPointsMultiplierHorizontalBox()
{
	GetWorldTimerManager().ClearTimer(HorizontalBoxTimer);
	GetWorldTimerManager().SetTimer(HorizontalBoxTimer, this, &AMainPlayerController::HidePointsMultiplierHorizontalBox, HorizontalBarDisplayTime);
	
	BaseHUD = BaseHUD == nullptr ? Cast<ABaseHUD>(GetHUD()) : BaseHUD;
	if (BaseHUD && BaseHUD->MainPlayerOverlayRef && BaseHUD->MainPlayerOverlayRef->HorizontalBoxPointsMultiplier)
	{
		BaseHUD->MainPlayerOverlayRef->HorizontalBoxPointsMultiplier->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::HidePointsMultiplierHorizontalBox()
{
	BaseHUD = BaseHUD == nullptr ? Cast<ABaseHUD>(GetHUD()) : BaseHUD;
	if (BaseHUD && BaseHUD->MainPlayerOverlayRef && BaseHUD->MainPlayerOverlayRef->HorizontalBoxPointsMultiplier)
	{
		BaseHUD->MainPlayerOverlayRef->HorizontalBoxPointsMultiplier->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::PlayHorizontalBoxPointsAnim()
{
	BaseHUD = BaseHUD == nullptr ? Cast<ABaseHUD>(GetHUD()) : BaseHUD;
	if (BaseHUD && BaseHUD->MainPlayerOverlayRef && BaseHUD->MainPlayerOverlayRef->HorizontalBoxPointsAnim)
	{
		BaseHUD->MainPlayerOverlayRef->PlayAnimation(BaseHUD->MainPlayerOverlayRef->HorizontalBoxPointsAnim);
	}
}

void AMainPlayerController::SetHUDRoundCountdown(float CountdownTime)
{
	BaseHUD = BaseHUD == nullptr ? Cast<ABaseHUD>(GetHUD()) : BaseHUD;
	if (BaseHUD && BaseHUD->MainPlayerOverlayRef && BaseHUD->MainPlayerOverlayRef->TxtRoundCountdown)
	{
		const int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		const int32 Seconds = CountdownTime - Minutes * 60;
		
		const FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		BaseHUD->MainPlayerOverlayRef->TxtRoundCountdown->SetText(FText::FromString(CountdownText));
	}
}
