// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/MainPlayerOverlay.h"

void ABaseHUD::DrawHUD()
{
	Super::DrawHUD();

	if(GEngine)
	{
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
		
		if(HUDCrosshairStruct.CrosshairsCenter)
		{
			DrawCrossHair(HUDCrosshairStruct.CrosshairsCenter, ViewportCenter,  HUDCrosshairStruct.Crosshairscolor);
		}
		if(HUDCrosshairStruct.CrosshairsLeft)
		{
			DrawCrossHair(HUDCrosshairStruct.CrosshairsLeft, ViewportCenter,  HUDCrosshairStruct.Crosshairscolor);
		}
		if(HUDCrosshairStruct.CrosshairsRight)
		{
			DrawCrossHair(HUDCrosshairStruct.CrosshairsRight, ViewportCenter,  HUDCrosshairStruct.Crosshairscolor);
		}
		if(HUDCrosshairStruct.CrosshairsBottom)
		{
			DrawCrossHair(HUDCrosshairStruct.CrosshairsBottom, ViewportCenter,  HUDCrosshairStruct.Crosshairscolor);
		}
		if(HUDCrosshairStruct.CrosshairsTop)
		{
			DrawCrossHair(HUDCrosshairStruct.CrosshairsTop, ViewportCenter,  HUDCrosshairStruct.Crosshairscolor);
		}
	}
}

void ABaseHUD::BeginPlay()
{
	Super::BeginPlay();

}

void ABaseHUD::AddCharacterOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if(PlayerController && IsValid(MainPlayerOverlayClass))
	{
		MainPlayerOverlayRef = CreateWidget<UMainPlayerOverlay> (PlayerController, MainPlayerOverlayClass);
		MainPlayerOverlayRef->AddToViewport();
	}
}

void ABaseHUD::DrawCrossHair(UTexture2D* Texture, FVector2D ViewportCenter, FLinearColor CrosshairColor)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();
	const FVector2D TextureDrawPoint(ViewportCenter.X - (TextureWidth / 2.f),
	ViewportCenter.Y - (TextureHeight / 2.f));

	DrawTexture(Texture, TextureDrawPoint.X,
		TextureDrawPoint.Y, TextureWidth, TextureHeight, 0.f, 0.f, 1.f,
		1.f, CrosshairColor);
	
}