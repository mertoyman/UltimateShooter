// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	Eir_Damaged UMETA(DisplayName = "Damaged"),
	Eir_Common UMETA(DisplayName = "Common"),
	Eir_Uncommon UMETA(DisplayName = "UnCommon"),
	Eir_Rare UMETA(DisplayName = "Rare"),
	Eir_Legendary UMETA(DisplayName = "Legendary"),

	Eir_MAX UMETA(DisplayName = "DefaultMAX")
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
	void OnSphereOverlap(
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

public:
	FORCEINLINE UWidgetComponent* GetPickupWidget() const { return PickupWidget; }
};
