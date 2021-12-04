// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	EIR_Damaged UMETA(DisplayName = "Damaged"),
	EIR_Common UMETA(DisplayName = "Common"),
	EIR_Uncommon UMETA(DisplayName = "UnCommon"),
	EIR_Rare UMETA(DisplayName = "Rare"),
	EIR_Legendary UMETA(DisplayName = "Legendary"),

	EIR_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Pickup UMETA(DisplayName = "Pickup"),
	EIS_EquipInterping UMETA(DisplayName = "EquipInterping"),
	EIS_PickedUp UMETA(DisplayName = "PickedUp"),
	EIS_Equipped UMETA(DisplayName = "Equipped"),
	EIS_Falling UMETA(DisplayName = "Falling"),

	EIS_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Ammo UMETA(DisplayName = "Ammo"),
	EIT_Weapon UMETA(DisplayName = "Weapon"),

	EIT_MAX UMETA(DisplayName = "DefaultMAX"),
};

UCLASS()
class ULTIMATESHOOTER_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Called when overlapping AreaSphere */
	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	/* Called when overlapping ends on AreaSphere */
	UFUNCTION()
	void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	/* Set the ActiveStars array of bools based on rarity */
	void SetActiveStars();

	/* Sets properties of the Item's components based on State */
	virtual void SetItemProperties(EItemState State);

	/* Called when ItemInterpTimer finished */
	void FinishInterping();

	/* Handles item interpolation when in the EquipInterping state */
	void ItemInterp(float DeltaTime);

	/* Get interp location based on the item type */ 
	FVector GetInterpLocation();
	
	void PlayPickupSound();

	virtual void InitializeCustomDepth();

	virtual void OnConstruction(const FTransform& Transform) override;

	void EnableGlowMaterial();

	void ResetPulseTimer();

	void StartPulseTimer();

	void UpdatePulse();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	/* Skeletal mesh for the item */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item Properties", meta= (AllowPrivateAccess=true))
	class USkeletalMeshComponent* ItemMesh;
	
	/* Line trace collides with box to show HUD widgets */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta= (AllowPrivateAccess=true))
	class UBoxComponent* CollisionBox;

	/* Pickup widget for when the player looks at the item */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta= (AllowPrivateAccess=true))
	class UWidgetComponent* PickupWidget;

	/* Enable item tracing when overlapped */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta= (AllowPrivateAccess=true))
	class USphereComponent* AreaSphere;

	/* The name which appears on the Pickup Widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta= (AllowPrivateAccess=true))
	FString ItemName;

	/* The number of items which appears on the Pickup Widget */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Properties", meta= (AllowPrivateAccess=true))
	int32 ItemCount;

	/* Item rarity - determines number of stars in Pickup Widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Properties", meta= (AllowPrivateAccess=true))
	EItemRarity ItemRarity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item Properties", meta= (AllowPrivateAccess=true))
	TArray<bool> ActiveStars;

	/* State of the Item */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item Properties", meta= (AllowPrivateAccess=true))
	EItemState ItemState;

	/* The curve asset to use for the Item's Z location when interping */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item Properties", meta= (AllowPrivateAccess=true))
	class UCurveFloat* ItemZCurve;

	/* Starting location when interping begins */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item Properties", meta= (AllowPrivateAccess=true))
	FVector ItemInterpStartLocation;

	/* Target interp location in front of the camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item Properties", meta= (AllowPrivateAccess=true))
	FVector CameraTargetLocation;

	/* True when interping */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item Properties", meta= (AllowPrivateAccess=true))
	bool bInterping;

	/* Plays when we start interping */
	FTimerHandle ItemInterpTimer;

	/* Duration of the curve and timer */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item Properties", meta= (AllowPrivateAccess=true))
	float ZCurveTime;

	/* Pointer to the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item Properties", meta= (AllowPrivateAccess=true))
	class AShooterCharacter* Character;

	/* X and Y for the Item while interping in the EquipInterping state */
	float ItemInterpX;
	float ItemInterpY;

	/* Initial Yaw offset between the camera and the interping item */ 
	float InterpInitialYawOffset;

	/* Curve used to scale item when interping */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item Properties", meta= (AllowPrivateAccess=true))
	UCurveFloat* ItemScaleCurve;

	/* Sound play when Item is picked up */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Properties", meta= (AllowPrivateAccess=true))
	class USoundCue* PickupSound;

	/* Sound play when Item is equipped */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Properties", meta= (AllowPrivateAccess=true))
	class USoundCue* EquipSound;

	/* Enum for the type of item this Item is */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Properties", meta= (AllowPrivateAccess=true))
	EItemType ItemType;

	/* Index of the Interp location this item is interping to */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item Properties", meta= (AllowPrivateAccess=true))
	int32 InterpLocIndex;

	/* Index of the material we'd like to change at runtime */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Properties", meta= (AllowPrivateAccess=true))
	int32 MaterialIndex;

	/* Dynamic instance we can change t runtime */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item Properties", meta= (AllowPrivateAccess=true))
	UMaterialInstanceDynamic* DynamicMaterialInstance;

	/* Material instance used with the Dynamic Material Instance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item Properties", meta= (AllowPrivateAccess=true))
	UMaterialInstance* MaterialInstance;

	bool bCanChangeCustomDepth;

	/* Curve to drive the dynamic material parameters */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item Properties", meta= (AllowPrivateAccess=true))
	class UCurveVector* PulseCurve;

	/* Curve to drive the dynamic material parameters */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item Properties", meta= (AllowPrivateAccess=true))
	UCurveVector* InterpPulseCurve;

	FTimerHandle PulseTimer;

	/* Time for the PulseTimer */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item Properties", meta= (AllowPrivateAccess=true))
	float PulseCurveTime;

	UPROPERTY(VisibleAnywhere, Category="Item Properties", meta= (AllowPrivateAccess=true))
	float GlowAmount;

	UPROPERTY(VisibleAnywhere, Category="Item Properties", meta= (AllowPrivateAccess=true))
	float FresnelExponent;

	UPROPERTY(VisibleAnywhere, Category="Item Properties", meta= (AllowPrivateAccess=true))
	float FresnelReflectFraction;

	/* Background for this item in the Inventory */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory", meta= (AllowPrivateAccess=true))
	UTexture2D* IconBackground;
	
	/* Icon for this item in the Inventory */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory", meta= (AllowPrivateAccess=true))
	UTexture2D* IconItem;

public:
	FORCEINLINE UWidgetComponent* GetPickupWidget() const { return PickupWidget; }
	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; }
	FORCEINLINE UBoxComponent* GetCollisionBox() const { return CollisionBox; }
	FORCEINLINE EItemState GetItemState() const {return ItemState;}
	FORCEINLINE USkeletalMeshComponent* GetItemMesh() const { return ItemMesh; }
	FORCEINLINE USoundCue* GetPickupSound() const { return PickupSound; }
	FORCEINLINE USoundCue* GetEquipSound() const { return EquipSound; }
	FORCEINLINE int32 GetItemCount() const { return ItemCount; }
	
	void SetItemState(EItemState State);
	/* Called from the AShooterCharacter class */
	void StartItemCurve(AShooterCharacter* Char);

	void PlayEquipSound();

	virtual void EnableCustomDepth();

	virtual void DisableCustomDepth();

	void DisableGlowMaterial();
};
