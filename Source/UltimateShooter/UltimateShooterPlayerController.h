// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UltimateShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ULTIMATESHOOTER_API AUltimateShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AUltimateShooterPlayerController();	

protected:
	virtual void BeginPlay() override;

	
private:
	/* Reference to the Overall HUD Overlay Blueprint Class */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Widgets, meta=(AllowPrivateAccess))
	TSubclassOf<class UUserWidget> HUDOverlayClass;

	/* Variable to hold the HUD Overlay Widget after creating it */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Widgets, meta=(AllowPrivateAccess))
	UUserWidget* HUDOverlay;
	
	
};
