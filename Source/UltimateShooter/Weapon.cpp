// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

// Sets default values
AWeapon::AWeapon() :
	ThrowWeaponTime(0.7f),
	bFalling(false),
	Ammo(30),
	MagazineCapacity(30),
	WeaponType(EWeaponType::EWT_SubmachineGun),
	AmmoType(EAmmoType::EAT_9mm),
	ReloadMontageSection("ReloadSMG"),
	ClipBoneName(TEXT("smg_clip"))
{
 	PrimaryActorTick.bCanEverTick = true;
}

void AWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Keep the Weapon upright
	if(GetItemState() == EItemState::EIS_Falling && bFalling)
	{
		const FRotator MeshRotation {0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.f};
		GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);
	}
}

void AWeapon::ThrowWeapon()
{
	const FRotator MeshRotation { 0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.f};
	GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);

	const FVector MeshForward { GetItemMesh()->GetForwardVector() };
	const FVector MeshRight (GetItemMesh()->GetRightVector());

	//Direction in which we throw the Weapon
	FVector ImpulseDirection = MeshRight.RotateAngleAxis(-20.f, MeshForward);

	const float RandomRotation { FMath::RandRange(0.f, 30.f) };
	ImpulseDirection = ImpulseDirection.RotateAngleAxis(RandomRotation, FVector{0.f, 0.f, 1.f});
	ImpulseDirection *= 2000.f;
	GetItemMesh()->AddImpulse(ImpulseDirection);
	
	bFalling = true;
	GetWorldTimerManager().SetTimer(ThrowWeaponTimer, this, &AWeapon::StopFalling, ThrowWeaponTime);

	EnableGlowMaterial();
}

void AWeapon::DecrementAmmo()
{
	if (Ammo - 1 <= 0)
	{
		Ammo = 0;
	}
	else
	{
		--Ammo;	
	}
}

void AWeapon::ReloadAmmo(int32 Amount)
{
	checkf(Ammo + Amount <= MagazineCapacity, TEXT("Attempted to reload with more than magazine capacity"));
	Ammo += Amount;
}

bool AWeapon::ClipIsFull()
{
	return Ammo >= MagazineCapacity;
}

void AWeapon::StopFalling()
{
	bFalling = false;
	SetItemState(EItemState::EIS_Pickup);
	StartPulseTimer(); 
}

void AWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	// Load the data in the Item Rarity Data Table

	//Path to the Item Rarity Data Table
	const FString WeaponTablePath(TEXT("DataTable'/Game/_Game/DataTables/WeaponDataTable.WeaponDataTable'"));
	UDataTable* WeaponTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *WeaponTablePath));

	if (WeaponTableObject)
	{
		FWeaponDataTable* WeaponRow = nullptr;
		switch (WeaponType)
		{
			case EWeaponType::EWT_SubmachineGun:
				WeaponRow = WeaponTableObject->FindRow<FWeaponDataTable>("SubmachineGun", "");
			break;

			case EItemRarity::EIR_Common:
				WeaponRow = WeaponTableObject->FindRow<FWeaponDataTable>("AssaultRifle", "");
			break;
			default: ;
		}

		if (WeaponRow)
		{
			AmmoType = WeaponRow->AmmoType;
			Ammo = WeaponRow->WeaponAmmo;
			MagazineCapacity = WeaponRow->MagazineCapacity;
			SetPickupSound(WeaponRow->PickupSound);
			SetEquipSound(WeaponRow->EquipSound);
			GetItemMesh()->SetSkeletalMesh(WeaponRow->ItemMesh);
			SetItemName(WeaponRow->ItemName);
			SetIconItem(WeaponRow->InventoryIcon);
			SetAmmoIcon(WeaponRow->AmmoIcon);
			SetClipBoneName(WeaponRow->ClipBoneName);
			SetReloadMontageSection(WeaponRow->ReloadMontageSection);

			SetMaterialInstance(WeaponRow->MaterialInstance);
			PreviousMaterialIndex = GetMaterialIndex();
			GetItemMesh()->SetMaterial(PreviousMaterialIndex, nullptr);
			SetMaterialIndex(WeaponRow->MaterialIndex);
		}

		if (GetMaterialInstance())
		{
			SetDynamicMaterialInstance(UMaterialInstanceDynamic::Create(GetMaterialInstance(), this));
			GetDynamicMaterialInstance()->SetVectorParameterValue("FresnelColor", GetGlowColor());
			GetItemMesh()->SetMaterial(GetMaterialIndex(), GetDynamicMaterialInstance());

			EnableGlowMaterial();
		}
	}
}



