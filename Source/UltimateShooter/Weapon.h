// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "AmmoType.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_SubmachineGun UMETA(DisplayName="SubmachineGun"),
	EWT_AssaultRifle UMETA(DisplayName="AssaultRifle"),

	EWT_DefaultMAX UMETA(DisplayName="DefaultMAX"),
};

USTRUCT(BlueprintType)
struct FWeaponDataTable: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MagazineCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundCue* PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* EquipSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* InventoryIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* AmmoIcon;
	
};

UCLASS()
class ULTIMATESHOOTER_API AWeapon : public AItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	virtual void Tick(float DeltaSeconds) override;

protected:
	void StopFalling();

	virtual void OnConstruction(const FTransform& Transform) override;

private:
	FTimerHandle ThrowWeaponTimer;
	float ThrowWeaponTime;
	bool bFalling;

	/* Ammo count for this weapon*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties", meta=(AllowPrivateAccess=true))
	int32 Ammo;

	/* Maximum ammo that our weapon can carry */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties", meta=(AllowPrivateAccess=true))
	int32 MagazineCapacity;
	
	/* The type of the weapon*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties", meta=(AllowPrivateAccess=true))
	EWeaponType WeaponType;

	/* The type of the ammo for this weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties", meta=(AllowPrivateAccess=true))
	EAmmoType AmmoType;

	/* FName for the Reload Montage Section */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties", meta=(AllowPrivateAccess=true))
	FName ReloadMontageSection;

	/* True when moving the clip while reloading */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon Properties", meta=(AllowPrivateAccess=true))
	bool bMovingClip;

	/* Name for the clip bone  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties", meta=(AllowPrivateAccess=true))
	FName ClipBoneName;

	/* Data table for weapon properties */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=DataTable, meta=(AllowPrivateAccess=true))
	UDataTable* WeaponDataTable;
	
public:

	FORCEINLINE int32 GetAmmo() const { return Ammo; }
	FORCEINLINE int32 GetMagazineCapacity() const { return MagazineCapacity; }
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }
	FORCEINLINE FName GetReloadMontageSection() const { return ReloadMontageSection; }
	FORCEINLINE FName GetClipBoneName() const { return ClipBoneName; }
	FORCEINLINE void SetMovingClip(const bool Move) { bMovingClip = Move; }

	/* Adds an impulse to the Weapon */
	void ThrowWeapon();

	/* Called from Character class when fired*/
	void DecrementAmmo();

	void ReloadAmmo(int32 Amount);

	bool ClipIsFull();
};
