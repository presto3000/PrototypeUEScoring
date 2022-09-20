// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

class UMainPlayerOverlay;

/** Set on each weapon */
USTRUCT(BlueprintType)
struct FHUDCrosshairStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsCenter;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsLeft;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsRight;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsTop;
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	UTexture2D* CrosshairsBottom;
	float CrosshairSpread;

	FLinearColor Crosshairscolor = FLinearColor{1.f,0,0};
	
};

/**
 * 
 */
UCLASS()
class PROTOTYPESCORING_API ABaseHUD : public AHUD
{
	GENERATED_BODY()
		
	virtual void DrawHUD() override;
	
public:	
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<UUserWidget> MainPlayerOverlayClass;
	UPROPERTY()
	UMainPlayerOverlay* MainPlayerOverlayRef;
	void AddCharacterOverlay();


protected:
	virtual void BeginPlay() override;
	void DrawCrossHair(UTexture2D* Texture, FVector2D ViewportCenter, FLinearColor CrosshairColor);

private:
	FHUDCrosshairStruct HUDCrosshairStruct;
public:
	FORCEINLINE void SetHUDCrosshairStruct(const FHUDCrosshairStruct& CrosshairStruct) {HUDCrosshairStruct = CrosshairStruct; }
};
