// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PrototypeScoring/MainPlayerController.h"
#include "ScoringGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPESCORING_API AScoringGameMode : public AGameMode
{
	GENERATED_BODY()
	AScoringGameMode();
protected:
	void SetHUDTime();
	
public:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere)
	float RoundTime = 180.f;

	uint32 CountdownInt = 0;
	uint32 SecondsLeft;
	UPROPERTY()
	AMainPlayerController* PC;
	UPROPERTY()
	ABaseHUD* BaseHUD;
};
