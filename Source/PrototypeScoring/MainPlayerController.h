// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class ABaseHUD;
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class PROTOTYPESCORING_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

	UPROPERTY()
	APlayerCharacter* PlayerCharacter;

	/********************************************************************
				Input callback functions
	********************************************************************/

	/************************************************
					 ACTIONS
	*************************************************/
	void OnAimButtonPressed();
	void OnAimButtonReleased();
	void OnFireButtonPressed();
	void OnFireButtonReleased();
	void OnSwapWeaponsPressed();
	void OnToggleAutomaticPressed();

	/************************************************
				 AXIS
	*************************************************/
	UFUNCTION()
	void OnMoveForwardBackwards(float AxisValue);
	UFUNCTION()
	void OnMoveRightLeft(float AxisValue);
	UFUNCTION()
	void OnLookUpDown(float AxisValue);
	UFUNCTION()
	void OnLookLeftRight(float AxisValue);

#pragma region Points
	
	// Current Score Value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Points)
	int32 ScoreValue;
	
	void SetScoreValue(const int32 Score);
	void SetPointsValue (const int32 PointsValue);
	void SetMultiplierValue(const int32 Multiplier);
	void ShowPointsMultiplierHorizontalBox();
	void HidePointsMultiplierHorizontalBox();
	void PlayHorizontalBoxPointsAnim();
	/** Time to display Horizontal bar once shot */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Points)
	float HorizontalBarDisplayTime = 4.f;
	FTimerHandle HorizontalBoxTimer;
	
	/** Player's StylePoints */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Points)
	int32 PlayerStylePoints = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Points)
	// Multiplier Values starting from x2 (x2, x3, x4...)
	TArray<int32> MultiplierValueArray {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Points)
	// Const times for each Multiplier 
	TArray<int32> MultiplierTimeValueArray {15, 10, 8, 7, 6, 5, 4, 3, 2, 1};

	// Multiplier Value
	int32 MultiplierValue;
	float Duration = 0;
	FTimerHandle TimerHandle_PointsMultiplier;
		
	
	void UpdateMultiplierTimer(int32 PlayerStylePoints);
	void UpdateDurationAndStylePoints();
	void ClearMultiplierTimer();

#pragma endregion Points

	void SetHUDRoundCountdown(float CountdownTime);
	
private:
	UPROPERTY()
	ABaseHUD* BaseHUD;
};
