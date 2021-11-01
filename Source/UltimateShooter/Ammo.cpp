// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammo.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

AAmmo::AAmmo()
{
	// Construct the AmmoMesh component and set it as the root	
	AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>("AmmoMesh");
	SetRootComponent(AmmoMesh);

	GetCollisionBox()->SetupAttachment(GetRootComponent());
	GetPickupWidget()->SetupAttachment(GetRootComponent());
	GetAreaSphere()->SetupAttachment(GetRootComponent());
}

void AAmmo::BeginPlay()
{
	Super::BeginPlay();
}

void AAmmo::SetItemProperties(EItemState State)
{
	Super::SetItemProperties(State);

	switch (State)
	{
		case EItemState::EIS_Pickup:
			// Set mesh properties
			AmmoMesh->SetSimulatePhysics(false);
			AmmoMesh->SetVisibility(true);
			AmmoMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
			AmmoMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;

		case EItemState::EIS_Equipped:
			// Set mesh properties
			AmmoMesh->SetSimulatePhysics(false);
			AmmoMesh->SetVisibility(true);
			AmmoMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
			AmmoMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;

		case EItemState::EIS_Falling:
			// Set mesh properties
			AmmoMesh->SetSimulatePhysics(true);
			AmmoMesh->SetVisibility(true);
			AmmoMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
			AmmoMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
			AmmoMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			break;

		case EItemState::EIS_EquipInterping:
			// Set mesh properties
			AmmoMesh->SetSimulatePhysics(false);
			AmmoMesh->SetVisibility(true);
			AmmoMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
			AmmoMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
			AmmoMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		default: ;
	}
}

void AAmmo::Tick(float delta_time)
{
	Super::Tick(delta_time);
}


