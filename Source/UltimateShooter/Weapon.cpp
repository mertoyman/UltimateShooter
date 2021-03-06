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
	ClipBoneName(TEXT("smg_clip")),
	SlideDisplacement(0.f),
	SlideDisplacementTime(0.2f),
	bMovingSlide(false),
	MaxSlideDisplacement(4.f),
	MaxRecoilRotation(20.f),
	bAutomatic(true)
{
 	PrimaryActorTick.bCanEverTick = true;
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
		FWeaponDataTable* WeaponDataRow = nullptr;
		switch (WeaponType)
		{
			case EWeaponType::EWT_SubmachineGun:
				WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>("SubmachineGun", "");
			break;

			case EWeaponType::EWT_AssaultRifle:
				WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>("AssaultRifle", "");
			break;

			case EWeaponType::EWT_Pistol:
				WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>("Pistol", "");
			break;
			
			default: ;
		}

		if (WeaponDataRow)
		{
			AmmoType = WeaponDataRow->AmmoType;
			Ammo = WeaponDataRow->WeaponAmmo;
			MagazineCapacity = WeaponDataRow->MagazineCapacity;
			SetPickupSound(WeaponDataRow->PickupSound);
			SetEquipSound(WeaponDataRow->EquipSound);
			GetItemMesh()->SetSkeletalMesh(WeaponDataRow->ItemMesh);
			SetItemName(WeaponDataRow->ItemName);
			SetIconItem(WeaponDataRow->InventoryIcon);
			SetAmmoIcon(WeaponDataRow->AmmoIcon);
			SetClipBoneName(WeaponDataRow->ClipBoneName);
			SetReloadMontageSection(WeaponDataRow->ReloadMontageSection);
			GetItemMesh()->SetAnimInstanceClass(WeaponDataRow->AnimBP);
			CrosshairsMiddle = WeaponDataRow->CrosshairsMiddle;
			CrosshairsRight = WeaponDataRow->CrosshairsRight;
			CrosshairsLeft = WeaponDataRow->CrosshairsLeft;
			CrosshairsBottom = WeaponDataRow->CrosshairsBottom;
			CrosshairsTop = WeaponDataRow->CrosshairsTop;
			AutoFireRate = WeaponDataRow->AutoFireRate;
			MuzzleFlash = WeaponDataRow->MuzzleFlash;
			FireSound = WeaponDataRow->FireSound;
			BoneToHide = WeaponDataRow->BoneToHide;
			GetItemMesh()->HideBoneByName(BoneToHide, EPhysBodyOp::PBO_None);
			bAutomatic = WeaponDataRow->bAutomatic;
			Damage = WeaponDataRow->Damage;
			HeadShotDamage = WeaponDataRow->HeadShotDamage;
			

			SetMaterialInstance(WeaponDataRow->MaterialInstance);
			PreviousMaterialIndex = GetMaterialIndex();
			GetItemMesh()->SetMaterial(PreviousMaterialIndex, nullptr);
			SetMaterialIndex(WeaponDataRow->MaterialIndex);
		}

		if (GetMaterialInstance())
		{
			SetDynamicMaterialInstance(UMaterialInstanceDynamic::Create(GetMaterialInstance(), this));
			GetDynamicMaterialInstance()->SetVectorParameterValue("FresnelColor", GetGlowColor());
			GetItemMesh()->SetMaterial(GetMaterialIndex(), GetDynamicMaterialInstance());

			EnableGlowMaterial();
		}

		if (BoneToHide != FName(""))
		{
			GetItemMesh()->HideBoneByName(BoneToHide, PBO_None);
		}
	}
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (BoneToHide != FName(""))
	{
		GetItemMesh()->HideBoneByName(BoneToHide, PBO_None);
	}
}

void AWeapon::FinishMovingSlide()
{
	bMovingSlide = false;
}

void AWeapon::UpdateSlideDisplacement()
{
	if (SlideDisplacementCurve && bMovingSlide)
	{
		const float ElapsedTime { GetWorldTimerManager().GetTimerElapsed(SlideTimer) };
		const float CurveValue { SlideDisplacementCurve->GetFloatValue(ElapsedTime) };
		SlideDisplacement = CurveValue * MaxSlideDisplacement;
		RecoilRotation = CurveValue * MaxRecoilRotation;
	}
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

	// Update slice on pistol
	UpdateSlideDisplacement();
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

void AWeapon::StartSlideTimer()
{
	bMovingSlide = true;
	
	GetWorldTimerManager().SetTimer(
		SlideTimer,
		this,
		&AWeapon::FinishMovingSlide,
		SlideDisplacementTime
		);
}

void AWeapon::StopFalling()
{
	bFalling = false;
	SetItemState(EItemState::EIS_Pickup);
	StartPulseTimer(); 
}
