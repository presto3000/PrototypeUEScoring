// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainPlayerOverlay.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PrototypeScoring/MainPlayerController.h"

void UMainPlayerOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	
	this->bIsFocusable = true;
	
	if (BorderScore)
	{
		BorderScore->SetVisibility(ESlateVisibility::Hidden);
	}

	if (Btn_Restart)
	{
		Btn_Restart->OnClicked.AddDynamic(this, &UMainPlayerOverlay::OnRestartBtnClicked);
	}
	if (Btn_Exit)
	{
		Btn_Exit->OnClicked.AddDynamic(this, &UMainPlayerOverlay::OneExitBtnClicked);
	}
	if (HorizontalBoxPointsMultiplier)
	{
		HorizontalBoxPointsMultiplier->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainPlayerOverlay::OnRestartBtnClicked()
{
    if(const UWorld* World = GetWorld())
    {
    	UKismetSystemLibrary::ExecuteConsoleCommand(World, TEXT("RestartLevel"));
    }
	PC = PC == nullptr ? Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)) : PC;
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PC->bShowMouseCursor = false;
	PC->bEnableClickEvents = false;
	PC->bEnableMouseOverEvents = false;
}

void UMainPlayerOverlay::OneExitBtnClicked()
{
	if(const UWorld* World = GetWorld())
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(World, TEXT("Quit"));
	}
}
