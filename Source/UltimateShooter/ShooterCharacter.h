// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "AmmoType.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"

#include "ShooterCharacter.generated.h"

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_FireTimerInProgress UMETA(DisplayName = "FireTimerInProgress"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),
	ECS_Equipping UMETA(DisplayName = "Equipping"),
	ECS_Stunned UMETA(DisplayName = "Stunned"),
	
	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};

USTRUCT(BlueprintType)
struct FInterpLocation
{
	GENERATED_BODY()

	/* Scene component to use for its location for interping */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SceneComponent;

	/* Number of items interping to/at this scene comp location */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ItemCount;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquipItemDelegate, int32, CurrentSlotIndex, int32, NewSlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHighlightIconDelegate, int32, SlotIndex, bool, bStartAnimation);

UCLASS()
class ULTIMATESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

	// Take combat damage
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

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

	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FHitResult& OutBeamLocation);
	
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

	void SelectButtonPressed();

	void SelectButtonReleased();
	
	/* Takes a weapon and attaches it to the mesh */
	void EquipWeapon(class AWeapon* WeaponToEquip, bool bSwapping = false);

	/* Drop weapon and let it fall to the ground */

	void DropWeapon();

	/* Drop currently equipped Weapon and equips TraceHitItem */ 
	void SwapWeapon(AWeapon* WeaponToSwap);

	void ReloadButtonPressed();

	/* Handle reloading of the weapon */
	void ReloadWeapon();
	
	/* Initialize ammo map with ammo values */
	void InitializeAmmoMap();

	/* Check to make sure out weapon has ammo */
	bool WeaponHasAmmo();

	/* Checks to see if we have ammo of the Equipped Weapon's ammo type */
	bool CarryingAmmo();

	/* Called from Animation Blueprint with Grab Clip notify */
	UFUNCTION(BlueprintCallable)
	void GrabClip();

	/* Called from Animation Blueprint with Replace Clip notify */
	UFUNCTION(BlueprintCallable)
	void ReplaceClip();

	void CrouchButtonPressed();

	virtual void Jump() override;

	/* Interps capsule half height when crouching/standing */
	void InterpCapsuleHalfHeight(float DeltaTime);

	void Aim();

	void StopAiming();

	UFUNCTION(BlueprintCallable)
	void PickupAmmo(class AAmmo* Ammo);

	void InitializeInterpLocations();

	void FKeyPressed();
	void OneKeyPressed();
	void TwoKeyPressed();
	void ThreeKeyPressed();
	void FourKeyPressed();
	void FiveKeyPressed();

	void ExchangeInventoryItems(int32 CurrentItemIndex, int32 NewItemIndex);

	int32 GetEmptyInventorySlot();

	void HighlightInventorySlot();

	UFUNCTION(BlueprintCallable)
	EPhysicalSurface GetPhysicalSurface();

	void PlayMeleeImpactSound();

	UFUNCTION(BlueprintCallable)
	void EndStun();

	void Die();

	void PlayDeathMontage();

	UFUNCTION(BlueprintCallable)
	void FinishDeath();


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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	float CameraDefaultFOV;

	/* Field of view value when zoomed in*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
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

	/* Montage for reload animations */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	UAnimMontage* ReloadMontage;

	/* Montage for equip animations */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	UAnimMontage* EquipMontage;
	

	UFUNCTION(BlueprintCallable)
	void FinishReloading();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	/* Transform of the clip when we first grab the clip during reloading */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Combat, meta= (AllowPrivateAccess = true))
	FTransform ClipTransform;

	/* Seen component to attach to the Character's hand during reloading */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Combat, meta= (AllowPrivateAccess = true))
	class USceneComponent* HandSceneComponent;

	/* True when crouching */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement, meta= (AllowPrivateAccess = true))
	bool bCrouching;

	/* Regular movement speed */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement, meta= (AllowPrivateAccess = true))
	float BaseMovementSpeed;

	/* Crouch walking speed */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement, meta= (AllowPrivateAccess = true))
	float CrouchMovementSpeed;

	/* Current half height of the capsule */
	float CurrentCapsuleHalfHeight;

	/* Half height of the capsule when not crouching */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Movement, meta= (AllowPrivateAccess = true))
	float StandingCapsuleHalfHeight;

	/* Half height of the capsule when crouching */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Movement, meta= (AllowPrivateAccess = true))
	float CrouchingCapsuleHalfHeight;

	/* Ground friction while not crouching */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Movement, meta= (AllowPrivateAccess = true))
	float BaseGroundFriction;

	/* Ground friction while crouching */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Movement, meta= (AllowPrivateAccess = true))
	float CrouchingGroundFriction;

	/* Used for knowing when the aiming button pressed */
	bool bAimingButtonPressed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta= (AllowPrivateAccess = true))
	USceneComponent* WeaponInterpComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta= (AllowPrivateAccess = true))
	USceneComponent* InterpComp1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta= (AllowPrivateAccess = true))
	USceneComponent* InterpComp2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta= (AllowPrivateAccess = true))
	USceneComponent* InterpComp3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta= (AllowPrivateAccess = true))
	USceneComponent* InterpComp4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta= (AllowPrivateAccess = true))
	USceneComponent* InterpComp5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta= (AllowPrivateAccess = true))
	USceneComponent* InterpComp6;

	/* Array of interp location structs */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta= (AllowPrivateAccess = true))
	TArray<FInterpLocation> InterpLocations;

	FTimerHandle PickupSoundTimer;
	FTimerHandle EquipSoundTimer;

	bool bShouldPlayPickupSound;
	bool bShouldPlayEquipSound;


	void ResetPickupSoundTimer();
	void ResetEquipSoundTimer();

	/* Time to wait before we can play another Pickup Sound */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Items", meta= (AllowPrivateAccess = true))
	float PickupSoundResetTime;

	/* Time to wait before we can play another Equip Sound */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Items", meta= (AllowPrivateAccess = true))
	float EquipSoundResetTime;

	/* Array of AItems for inventory */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory", meta= (AllowPrivateAccess = true))
	TArray<AItem*> Inventory;

	const int32 INVENTORY_CAPACITY{ 6 };

	/* Delegate for sending slot information to InventoryBar when equipping */
	UPROPERTY(BlueprintAssignable, Category="Delegates", meta= (AllowPrivateAccess = true))
	FEquipItemDelegate EquipItemDelegate;

	/* Delegate for sending slot information for playing the icon animation */
	UPROPERTY(BlueprintAssignable, Category="Delegates", meta= (AllowPrivateAccess = true))
	FHighlightIconDelegate HighlightIconDelegate;

	/* The index for currently highlighted slot */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory", meta= (AllowPrivateAccess = true))
	int32 HighlightedSlot;

	/* Character health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	float Health;

	/* Character max health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	float MaxHealth;

	/* Sound made when character gets hit by a melee attack */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	USoundCue* MeleeImpactSound;

	/* Blood particle emiited when character gets hit by a melee attack */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	UParticleSystem* BloodParticles;

	/* Hit react anim montage for when character is stunned */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	UAnimMontage* HitReactMontage;

	/* Chance of being stunned when hit by enemy */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	float StunChance;

	/* Montage containing different death animations */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	UAnimMontage* DeathMontage;

	/* True when we die */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Combat, meta= (AllowPrivateAccess = true))
	bool bDead;


public:
	/* Return CameraBoom subobject*/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/* Return FollowCamera subobject*/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE bool GetAiming() const { return bAiming; }
	FORCEINLINE int8 GetOverlappedItemCount() const { return OverlappedItemCount; }
	FORCEINLINE ECombatState GetCombatState() const { return CombatState; }
	FORCEINLINE void SetCombatState(ECombatState State) { CombatState = State; } 
	FORCEINLINE bool GetCrouching() const { return bCrouching; }
	FORCEINLINE FInterpLocation GetInterpLocation(int32 Index);
	FORCEINLINE bool GetShouldPlayPickupSound() const { return bShouldPlayPickupSound; }
	FORCEINLINE bool GetShouldPlayEquipSound() const { return bShouldPlayEquipSound; }
	FORCEINLINE USoundCue* GetMeleeImpactSound() const { return MeleeImpactSound; }
	FORCEINLINE UParticleSystem* GetBloodParticles() const { return BloodParticles; }
	
	UFUNCTION(BlueprintCallable)
	float GetCrosshairSpreadMultiplier() const;


	/* Adds/subtracts to/from OverlappedItemCount and updates bShouldTraceForItems */
	void IncrementOverlappedItemCount(int8 Amount);

	void GetPickupItem(AItem* Item);

	// Returns the index in InterpL	ocations array with the lowest ItemCount
	int32 GetInterpLocationIndex();

	void IncrementInterpLocItemCount(int32 Index, int32 Amount);

	void StartPickupSoundTimer();
	void StartEquipSoundTimer();

	void UnHighlightInventorySlot();

	FORCEINLINE AWeapon* GetEquippedWeapon() const { return EquippedWeapon; }

	void Stun();

	FORCEINLINE float GetStunChance() const { return StunChance; }

	void SwitchLevel(FName LevelName);

	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame(bool SetPosition);
};
