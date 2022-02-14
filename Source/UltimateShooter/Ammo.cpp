// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammo.h"

#include "ShooterCharacter.h"
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

	AmmoCollisionSphere = CreateDefaultSubobject<USphereComponent>("AmmoCollisionSphere");
	AmmoCollisionSphere->SetupAttachment(GetRootComponent());
	AmmoCollisionSphere->SetSphereRadius(50.f);
}


void AAmmo::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Load the data in the Item Rarity Data Table

	//Path to the Item Rarity Data Table
	const FString AmmoTablePath(TEXT("DataTable'/Game/_Game/DataTables/AmmoDataTable.AmmoDataTable'"));
	UDataTable* AmmoTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *AmmoTablePath));

	if (AmmoTableObject)
	{
		FAmmoDataTable* AmmoDataRow = nullptr;
		switch (AmmoType)
		{
			case EAmmoType::EAT_9mm:
				AmmoDataRow = AmmoTableObject->FindRow<FAmmoDataTable>("9mm", "");
			break;

			case EAmmoType::EAT_AR:
				AmmoDataRow = AmmoTableObject->FindRow<FAmmoDataTable>("ARAmmo", "");
			break;
			
			default: ;
		}

		if (AmmoDataRow)
		{
			AmmoIconTexture = AmmoDataRow->AmmoIcon;
			AmmoPickupSound = AmmoDataRow->PickupSound;
			AmmoEquipSound = AmmoDataRow->EquipSound;
			AmmoMesh->SetStaticMesh(AmmoDataRow->AmmoMesh);
			AmmoName = AmmoDataRow->AmmoName;
		}
	}
}

void AAmmo::BeginPlay()
{
	Super::BeginPlay();

	AmmoCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AAmmo::AmmoSphereOverlap);
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

void AAmmo::AmmoSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		const auto OverlappedCharacter = Cast<AShooterCharacter>(OtherActor);
		if (OverlappedCharacter)
		{
			StartItemCurve(OverlappedCharacter);
			AmmoCollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	
}

void AAmmo::EnableCustomDepth()
{
	AmmoMesh->SetRenderCustomDepth(true);
}

void AAmmo::DisableCustomDepth()
{
	AmmoMesh->SetRenderCustomDepth(false);
}


