// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "MainPlayerOverlay.generated.h"

class AMainPlayerController;
/**
 * 
 */
UCLASS()
class PROTOTYPESCORING_API UMainPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TxtScore;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TxtScore_2;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TxtPoints;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TxtStylePointsMultiplier;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TxtRoundCountdown;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* BorderPlayerOverlay;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UBorder* BorderScore;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Btn_Restart;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Btn_Exit;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* HorizontalBoxPointsMultiplier;
	
	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim))
	UWidgetAnimation* HorizontalBoxPointsAnim;

	UFUNCTION()
	void OnRestartBtnClicked();
	UFUNCTION()
	void OneExitBtnClicked();
	
	UPROPERTY(BlueprintReadWrite)
	AMainPlayerController* PC;
};
