// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "AmmoType.h"
#include "Ammo.generated.h"

USTRUCT(BlueprintType)
struct FAmmoDataTable : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* AmmoIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* EquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* AmmoMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AmmoName;
	
};

/**
 * 
 */
UCLASS()
class ULTIMATESHOOTER_API AAmmo : public AItem
{
	GENERATED_BODY()

public:
	AAmmo();

protected:

	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	/* Override of SetItemProperties so we can set AmmoMesh properties */
	virtual void SetItemProperties(EItemState State) override;

	/* Called when overlapping AreaSphere */
	UFUNCTION()
	void AmmoSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	
private:

	/* Ammo Type for the Ammo */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AmmoProperties, meta=(AllowPrivateAccess=true))
	EAmmoType AmmoType;

	/* Mesh for the ammo pickup */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=AmmoProperties, meta=(AllowPrivateAccess=true))
	UStaticMeshComponent* AmmoMesh;

	/* Collision box for picking up the ammo when overlapped */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AmmoProperties, meta=(AllowPrivateAccess=true))
	class USphereComponent* AmmoCollisionSphere;
	
	/* The texture for the Ammo icon */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=DataTable, meta=(AllowPrivateAccess=true))
	UTexture2D* AmmoIconTexture;

	/* Pick up sound for the ammo type */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=DataTable, meta=(AllowPrivateAccess=true))
	class USoundCue* AmmoPickupSound;

	/* Equip sound for the ammo type */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=DataTable, meta=(AllowPrivateAccess=true))
	USoundCue* AmmoEquipSound;

	/* Name of the ammo item */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=DataTable, meta=(AllowPrivateAccess=true))
	FString AmmoName;
	
	/* Data table for ammo properties */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=DataTable, meta=(AllowPrivateAccess=true))
	UDataTable* AmmoDataTable;

public:
	FORCEINLINE UStaticMeshComponent* GetAmmoMesh() const { return AmmoMesh; }
	FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }
	FORCEINLINE UTexture2D* GetAmmoIcon() const { return AmmoIconTexture; }
	
	virtual void EnableCustomDepth() override;
	virtual void DisableCustomDepth() override;
};
