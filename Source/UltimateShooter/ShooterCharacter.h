// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

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

	/** Called when fire button is pressed */
	void FireWeapon();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess = true))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess = true))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta= (AllowPrivateAccess = true))
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta= (AllowPrivateAccess = true))
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	class USoundCue* FireSound;
	
public:

	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};
