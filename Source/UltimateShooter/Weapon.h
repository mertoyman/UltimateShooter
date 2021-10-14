// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class  EWeaponType : uint8
{
	EWT_SubmachineGun UMETA(DisplayName="SubmachineGun"),
	EWT_AssaultRifle UMETA(DisplayName="AssaultRifle"),

	EWT_DefaultMAX UMETA(DisplayName="DefaultMAX"),
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

private:
	FTimerHandle ThrowWeaponTimer;
	float ThrowWeaponTime;
	bool bFalling;

	/* Ammo count for this weapon*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties", meta=(AllowPrivateAccess=true))
	int32 Ammo;

	/* The type of the weapon*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Properties", meta=(AllowPrivateAccess=true))
	EWeaponType WeaponType;

public:
	/* Adds an impulse to the Weapon */
	void ThrowWeapon();

	FORCEINLINE int32 GetAmmo() const { return Ammo; }

	/* Called from Character class when fired*/
	void DecrementAmmo();

	EWeaponType GetWeaponType() const { return WeaponType; }
};
