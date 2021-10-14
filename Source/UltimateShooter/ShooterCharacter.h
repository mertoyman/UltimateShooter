// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	
	EAT_9mm UMETA(DisplayName = "9mm"),
	EAT_AR UMETA(DisplayName = "AssaultRifle"),

	EAT_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_FireTimerInProgress UMETA(DisplayName = "FireTimerInProgress"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),
	
	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class ULTIMATESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

	//Called for forward/backwards inputs
	void MoveForward(float Value);

	//Called for right/left inputs
	void MoveRight(float Value);

	/*Called for turn right/left inputs at a given rate
	 *@param Rate This is a normalized rate, i.e. 1.0 means 100% of desired rate
	*/
	void TurnAtRate(float Rate);

	/*Called for look up/down inputs at a given rate
	*@param Rate This is a normalized rate, i.e. 1.0 means 100% of desired rate
	*/
    void LookUpAtRate(float Rate);

	/*Rotate controller based on mouse X movement
	 *@param Value This is the input value from mouse movement
	*/
	void Turn(float Value);

	/*Rotate controller based on mouse Y movement
	 *@param Value This is the input value from mouse movement
	*/
	void LookUp(float Value);

	/** Called when fire button is pressed */
	void FireWeapon();

	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation);
	
	/* Set bAiming to true or false with button press*/
	void AimingButtonPressed();
	void AimingButtonReleased();

	void SetCameraFOV(float DeltaTime);

	void SetLookRates();

	void CalculateCrosshairSpread(float DeltaTime);

	UFUNCTION()
	void FinishCrosshairBulletFire();

	void StartCrosshairBulletFire();

	void FireButtonPressed();

	void FireButtonReleased();

	void PlayFireSound();

	void SendBullet();
	void PlayHipFireMontage();

	void StartFireTimer();

	UFUNCTION()
	void AutoFireReset();

	/* Line trace for items under the crosshairs */
	bool TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation);

	/* Trace for items if overlapped item count is greater than zero */
	void TraceForItems();

	/* Spawms the default weapon and equips it */
	AWeapon* SpawnDefaultWeapon() const;

	/* Takes a weapon and attaches it to the mesh */
	void EquipWeapon(class AWeapon* WeaponToEquip);

	/* Drop weapon and let it fall to the ground */
	void DropWeapon();

	void SelectButtonPressed();

	void SelectButtonReleased();

	/* Drop currently equipped Weapon and equips TraceHitItem */ 
	void SwapWeapon(AWeapon* WeaponToSwap);

	/* Initialize ammo map with ammo values */
	void InitializeAmmoMap();

	/* Check to make sure out weapon has ammo */
	bool WeaponHasAmmo();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess = true))
	class USpringArmComponent* CameraBoom;

	/* Camera that follows the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess = true))
	class UCameraComponent* FollowCamera;

	/* Base turn rate, in deg/sec. Other scaling may affect the final turn rate */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Camera, meta= (AllowPrivateAccess = true))
	float BaseTurnRate;

	/* Base look up/down rate, in deg/sec. Other scaling may affect the final turn rate */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Camera, meta= (AllowPrivateAccess = true))
	float BaseLookUpRate;

	/* Turn rate when not aiming */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	float HipTurnRate;

	/* Look up rate when not aiming */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	float HipLookUpRate;

	/* Turn rate when aiming */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	float AimingTurnRate;

	/* Look up rate when aiming */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	float AimingLookUpRate;

	/* Mouse turn rate when not aiming */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true, ClampMin = 0.f, ClampMax = 1.f))
	float MouseHipTurnRate;

	/* Mouse look up rate when not aiming */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true, ClampMin = 0.f, ClampMax = 1.f))
	float MouseHipLookUpRate;
	
	/* Mouse turn rate when aiming */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true, ClampMin = 0.f, ClampMax = 1.f))
	float MouseAimingTurnRate;

	/* Mouse look up rate when aiming */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true, ClampMin = 0.f, ClampMax = 1.f))
	float MouseAimingLookUpRate;

	/** Randomized gunshot sound cue */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	class USoundCue* FireSound;

	/** Flash spawned at BarrelSocket */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	class UParticleSystem* MuzzleFlash;

	/** Montage for firing weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	class UAnimMontage* HipFireMontage;

	/** Particles spawned upon bullet impact */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	UParticleSystem* ImpactParticles;

	/** Smoke trail for the bullet */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	UParticleSystem* BeamParticles;

	/* True when aiming*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = true))
	bool bAiming;

	/* Default camera field of view value*/
	float CameraDefaultFOV;

	/* Field of view value when zoomed in*/
	float CameraZoomedFOV;

	/*Current field of view this frame*/
	float CameraCurrentFOV;

	/* Interpolation speed for zooming when aiming*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = true))
	float InterpSpeed;

	/* Determines the spread of the crosshairs */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = true))
	float CrosshairSpreadMultiplier;

	/* Velocity component for scrosshairs spread*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = true))
	float CrosshairVelocityFactor;

	/* In air component for scrosshairs spread*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = true))
	float CrosshairInAirFactor;

	/* Aim component for scrosshairs spread*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = true))
	float CrosshairAimFactor;

	/* Shooting component for scrosshairs spread*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = true))
	float CrosshairShootingFactor;

	/* Amount of time in seconds between each spread and shrink of the crosshair when shooting */
	float ShootTimeDuration;

	/* True when firing bullet */
	bool bFiringBullet;

	/* Sets a timer between crosshair spreading */
	FTimerHandle CrosshairShootTimer;

	/* True when we can fire. False when waiting for the timer */
	bool bShouldFire;

	/* Rate of automatic gun fire */
	float AutomaticFireRate;

	/* Sets a timer between gunshots */
	FTimerHandle AutoFireTimer;

	bool bFireButtonPressed;

	/* True if we should trace every frame for items */
	bool bShouldTraceForItems;

	/* Number of overlapped AItems */
	int8 OverlappedItemCount;

	/* The AItem we hit last frame */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Items, meta=(AllowPrivateAccess=true))
	class AItem* TraceHitItemLastFrame;

	/* Currently equipped weapon */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= Combat, meta=(AllowPrivateAccess=true))
	class AWeapon* EquippedWeapon;

	/* Set this in Blueprints for the default Weapon class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Combat, meta=(AllowPrivateAccess=true))
	TSubclassOf<AWeapon> DefaultWeaponClass;

	/* The Item currently hit by our trace in TraceForItems (could be null) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Combat, meta=(AllowPrivateAccess=true))
	AItem* TraceHitItem;

	/* Distance outward from the camera for the interp destination */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Items, meta=(AllowPrivateAccess=true))
	float CameraInterpDistance;

	/* Distance upward from the camera for the interp destination */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Items, meta=(AllowPrivateAccess=true))
	float CameraInterpElevation;

	/* Map to keep track of ammo of the different ammo types */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Items, meta=(AllowPrivateAccess=true))
	TMap<EAmmoType, int32> AmmoMap;

	/* Starting amount of 9mm ammo */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Items, meta=(AllowPrivateAccess=true))
	int32 Starting9mmAmmo;

	/* Starting amount of AR ammo */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Items, meta=(AllowPrivateAccess=true))
	int32 StartingARAmmo;

	/* Combat state can only fire or reload when unoccupied */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Combat, meta=(AllowPrivateAccess=true))
	ECombatState CombatState;

public:
	/* Return CameraBoom subobject*/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	/* Return FollowCamera subobject*/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE bool GetAiming() const { return bAiming; }

	UFUNCTION(BlueprintCallable)
	float GetCrosshairSpreadMultiplier() const;

	FORCEINLINE int8 GetOverlappedItemCount() const { return OverlappedItemCount; }

	/* Adds/subtracts to/from OverlappedItemCount and updates bShouldTraceForItems */
	void IncrementOverlappedItemCount(int8 Amount);

	FVector GetCameraInterpLocation();

	void GetPickupItem(AItem* Item);
};
