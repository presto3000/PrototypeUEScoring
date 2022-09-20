// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/ScoringGameMode.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/BaseHUD.h"
#include "UI/MainPlayerOverlay.h"

AScoringGameMode::AScoringGameMode()
{
	SecondsLeft = RoundTime;
}
void AScoringGameMode::BeginPlay()
{
	Super::BeginPlay();

	PC = PC == nullptr ? Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)) : PC;
}
void AScoringGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (SecondsLeft > 1)
	{
		SetHUDTime();
	}
	else if (SecondsLeft == 1)
	{
		// Show Score Border in Overlay Widget
		PC = PC == nullptr ? Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)) : PC;
		BaseHUD = BaseHUD == nullptr ? Cast<ABaseHUD>(PC->GetHUD()) : BaseHUD;
		if (BaseHUD && BaseHUD->MainPlayerOverlayRef && BaseHUD->MainPlayerOverlayRef->BorderPlayerOverlay && BaseHUD->MainPlayerOverlayRef->BorderScore)
		{
			BaseHUD->MainPlayerOverlayRef->BorderPlayerOverlay->SetVisibility(ESlateVisibility::Hidden);
			BaseHUD->MainPlayerOverlayRef->BorderScore->SetVisibility(ESlateVisibility::Visible);
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PC, BaseHUD->MainPlayerOverlayRef);
		}
	}
}


void AScoringGameMode::SetHUDTime()
{
	if (const UWorld* World = GetWorld())
	{
		SecondsLeft = FMath::CeilToInt(RoundTime - World->GetTimeSeconds());
		
			// Different only when 1s has passed
			if (CountdownInt != SecondsLeft)
			{
				PC = PC == nullptr ? Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)) : PC;
				PC->SetHUDRoundCountdown(RoundTime - GetWorld()->GetTimeSeconds());
			}
			CountdownInt = SecondsLeft;
	}
}



