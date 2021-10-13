// Fill out your copyright notice in the Description page of Project Settings.


#include "UltimateShooterPlayerController.h"

#include "Blueprint/UserWidget.h"

AUltimateShooterPlayerController::AUltimateShooterPlayerController()
{
}

void AUltimateShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//Check our HUDOverlayClass TSubclassOf Variable
	if (HUDOverlayClass)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayClass);
		if (HUDOverlay)
		{
			HUDOverlay->AddToViewport();
			HUDOverlay->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
