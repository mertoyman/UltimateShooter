// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "AmmoType.h"
#include "WeaponType.h"
#include "Weapon.generated.h"

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* MaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaterialIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ClipBoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ReloadMontageSection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsMiddle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsBottom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CrosshairsTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AutoFireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BoneToHide;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutomatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeadShotDamage;
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

	virtual void BeginPlay() override;

	void FinishMovingSlide();
	void UpdateSlideDisplacement();

private:
	FTimerHandle ThrowWeaponTimer;
	float ThrowWeaponTime;
	bool bFalling;

	/* Ammo count for this weapon*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category="Weapon Properties", meta=(AllowPrivateAccess=true))
	int32 Ammo;

	/* Maximum ammo that our weapon can carry */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category="Weapon Properties", meta=(AllowPrivateAccess=true))
	int32 MagazineCapacity;
	
	/* The type of the weapon*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties", meta=(AllowPrivateAccess=true))
	EWeaponType WeaponType;

	/* The type of the ammo for this weapon */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category="Weapon Properties", meta=(AllowPrivateAccess=true))
	EAmmoType AmmoType;

	/* FName for the Reload Montage Section */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category="Weapon Properties", meta=(AllowPrivateAccess=true))
	FName ReloadMontageSection;

	/* True when moving the clip while reloading */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Weapon Properties", meta=(AllowPrivateAccess=true))
	bool bMovingClip;

	/* Name for the clip bone  */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category="Weapon Properties", meta=(AllowPrivateAccess=true))
	FName ClipBoneName;

	/* Data table for weapon properties */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=DataTable, meta=(AllowPrivateAccess=true))
	UDataTable* WeaponDataTable;

	int32 PreviousMaterialIndex;

	/* Textures for the weapon crosshairs */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DataTable", meta=(AllowPrivateAccess=true))
	UTexture2D* CrosshairsMiddle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DataTable", meta=(AllowPrivateAccess=true))
	UTexture2D* CrosshairsRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DataTable", meta=(AllowPrivateAccess=true))
	UTexture2D* CrosshairsLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DataTable", meta=(AllowPrivateAccess=true))
	UTexture2D* CrosshairsBottom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DataTable", meta=(AllowPrivateAccess=true))
	UTexture2D* CrosshairsTop;

	/* The speed at which automatic fire happens */ 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DataTable", meta=(AllowPrivateAccess=true))
	float AutoFireRate;

	/* Particle system spawned at the BarrelSocket */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DataTable", meta=(AllowPrivateAccess=true))
	class UParticleSystem* MuzzleFlash;

	/* Sound played when the weapon is fired */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DataTable", meta=(AllowPrivateAccess=true))
	USoundCue* FireSound;

	/* Name of the bone to hide on the weapon mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DataTable", meta=(AllowPrivateAccess=true))
	FName BoneToHide;

	/* Amount that the slide is pushed back during the pistol fire */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pistol", meta=(AllowPrivateAccess=true))
	float SlideDisplacement;

	/* Curve for the slide displacement */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pistol", meta=(AllowPrivateAccess=true))
	UCurveFloat* SlideDisplacementCurve;

	/* Timer handle for updating SlidePlacement */
	FTimerHandle SlideTimer;

	/* Time for displacing the slide during pistol fire */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pistol", meta=(AllowPrivateAccess=true))
	float SlideDisplacementTime;

	/* True when moving the pistol slide */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pistol", meta=(AllowPrivateAccess=true))
	bool bMovingSlide;

	/* Max distance for the slide on the pistol */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pistol", meta=(AllowPrivateAccess=true))
	float MaxSlideDisplacement;

	/* Max rotation for pistol recoil */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pistol", meta=(AllowPrivateAccess=true))
	float MaxRecoilRotation;

	/* Amount that the pistol will rotate during the pistol fire */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pistol", meta=(AllowPrivateAccess=true))
	float RecoilRotation;

	/* True for auto gunfire */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Weapon Properties", meta=(AllowPrivateAccess=true))
	bool bAutomatic;

	/* Amount of damage caused by a bullet */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Weapon Properties", meta=(AllowPrivateAccess=true))
	float Damage;

	/* Amount of damage when a bullet hits the head */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Weapon Properties", meta=(AllowPrivateAccess=true))
	float HeadShotDamage;
	
public:

	FORCEINLINE int32 GetAmmo() const { return Ammo; }
	FORCEINLINE int32 GetMagazineCapacity() const { return MagazineCapacity; }
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }
	FORCEINLINE FName GetReloadMontageSection() const { return ReloadMontageSection; }
	FORCEINLINE void SetReloadMontageSection(FName Section) { ReloadMontageSection = Section; }
	FORCEINLINE FName GetClipBoneName() const { return ClipBoneName; }
	FORCEINLINE void SetClipBoneName(FName Name) { ClipBoneName = Name; }
	FORCEINLINE void SetMovingClip(const bool Move) { bMovingClip = Move; }
	FORCEINLINE float GetAutoFireRate() const { return AutoFireRate; }
	FORCEINLINE UParticleSystem* GetMuzzleFlash() const { return MuzzleFlash; }
	FORCEINLINE USoundCue* GetFireSound() const { return FireSound; }
	FORCEINLINE bool GetAutomatic() const { return bAutomatic; }
	FORCEINLINE float GetDamage() const { return Damage; }
	FORCEINLINE float GetHeadshotDamage() const { return HeadShotDamage; }
	
	/* Adds an impulse to the Weapon */
	void ThrowWeapon();

	/* Called from Character class when fired*/
	void DecrementAmmo();

	void ReloadAmmo(int32 Amount);

	bool ClipIsFull();

	void StartSlideTimer();
};
