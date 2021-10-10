// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"

#include "DrawDebugHelpers.h"
#include "Item.h"
#include "Weapon.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
AShooterCharacter::AShooterCharacter() :
	// Base rates for turning/looking up
	BaseTurnRate(45.f),
	BaseLookUpRate(45.f),
	// Turn rates for aiming/not aiming
	HipTurnRate(90.f),
	HipLookUpRate(90.f),
	AimingTurnRate(20.f),
	AimingLookUpRate(20.f),
	//Mouse look sensitivity scale factors
	MouseHipTurnRate(1.f),
	MouseHipLookUpRate(1.f),
	MouseAimingTurnRate(.2f),
	MouseAimingLookUpRate(.2f),
	//True when aiming the weapon
	bAiming(false),
	//Camera field of view values
	CameraDefaultFOV(0.f), //set in BeginPlay
	CameraZoomedFOV(35.f),
	CameraCurrentFOV(0.f),
	InterpSpeed(20.f),
	//Crosshair spread factors
	CrosshairSpreadMultiplier(0.f),
	CrosshairVelocityFactor(0.f),
	CrosshairInAirFactor(0.f),
	CrosshairAimFactor(0.f),
	CrosshairShootingFactor(0.f),
	//Bullet fire timer variables
	ShootTimeDuration(0.05f),
	bFiringBullet(false),
	//Automatic fire variables
	bShouldFire(true),
	AutomaticFireRate(.1f),
	bFireButtonPressed(false),
	//Item trace variable
	bShouldTraceForItems(false)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 180.f;
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 70.f);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false; //Camera does not rotate relative to arm

	//Do not rotate when controller rotates. Let the controller only affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false; //Character does not move in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f); //... at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (FollowCamera)
	{
		CameraDefaultFOV = GetFollowCamera()->FieldOfView;
		CameraCurrentFOV = CameraDefaultFOV;
	}

	// Spawn the default weapon and attach to the mesh
	SpawnDefaultWeapon();
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Handle interpolation for zoom when aiming
	SetCameraFOV(DeltaTime);

	// Change look sensitivity based on aiming
	SetLookRates();

	// Calculate crosshair spread multiplier
	CalculateCrosshairSpread(DeltaTime);

	// Check OverlappedItemCount, then trace for items
	TraceForItems();
	
}

void AShooterCharacter::MoveForward(float Value)
{
	if (Controller && Value != 0.f)
	{
		//find out which way is forward
		const FRotator Rotation{Controller->GetControlRotation()};
		const FRotator YawRotation{0, Rotation.Yaw, 0};

		const FVector Direction{FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X)};
		AddMovementInput(Direction, Value);
	}
}

void AShooterCharacter::MoveRight(float Value)
{
	//find out which way is right
	const FRotator Rotation{Controller->GetControlRotation()};
	const FRotator YawRotation{0, Rotation.Yaw, 0};

	const FVector Direction{FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y)};

	AddMovementInput(Direction, Value);
}

void AShooterCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds()); // deg/sec * sec/frame
}

void AShooterCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Turn(float Value)
{
	float TurnScaleFactor{};

	if (bAiming)
	{
		TurnScaleFactor = MouseAimingTurnRate;
	}
	else
	{
		TurnScaleFactor = MouseHipTurnRate;
	}

	AddControllerYawInput(Value * TurnScaleFactor);
}

void AShooterCharacter::LookUp(float Value)
{
	float LookUpScaleFactor{};

	if (bAiming)
	{
		LookUpScaleFactor = MouseAimingLookUpRate;
	}
	else
	{
		LookUpScaleFactor = MouseHipLookUpRate;
	}

	AddControllerPitchInput(Value * LookUpScaleFactor);
}

void AShooterCharacter::TraceForItems()
{
	if (bShouldTraceForItems)
	{
		FHitResult ItemTraceResult;
		FVector HitLocation;
		TraceUnderCrosshairs(ItemTraceResult, HitLocation);
		if (ItemTraceResult.bBlockingHit)
		{
			AItem* HitItem = Cast<AItem>(ItemTraceResult.Actor);
			if (HitItem && HitItem->GetPickupWidget())
			{
				//Show item's pickup widget
				HitItem->GetPickupWidget()->SetVisibility(true);
			}

			//We hit an AItem last frame
			if (TraceHitItemLastFrame)
			{
				if (HitItem != TraceHitItemLastFrame)
				{
					//We are hitting a different AItem this frame from last frame
					//Or AItem is null.
					TraceHitItemLastFrame->GetPickupWidget()->SetVisibility(false);
				}
			}

			//Store a reference to HitItem for next frame
			TraceHitItemLastFrame = HitItem;
		}
	}
	else if(TraceHitItemLastFrame)
	{
		//No longer overlapping any items
		TraceHitItemLastFrame->GetPickupWidget()->SetVisibility(false);
	}
}

void AShooterCharacter::SpawnDefaultWeapon()
{
	// Check the TSubclassOf variables
	if (DefaultWeaponClass)
	{
		// Spawn the weapon
		AWeapon* DefaultWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultWeaponClass);
		// Get the Hand Socket
		const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName("RightHandSocket");
		if (HandSocket)
		{
			// Attach the weapon to the hand socket RightHandSocket
			HandSocket->AttachActor(DefaultWeapon, GetMesh());
		}

		// Set EquippedWeapon to the newly spawned Weapon
		EquippedWeapon = DefaultWeapon;
	}
}

bool AShooterCharacter::GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation)
{
	// Check for crosshair trace hit
	FHitResult CrosshairHitResult;
	bool bCrosshairHit = TraceUnderCrosshairs(CrosshairHitResult, OutBeamLocation);

	if (bCrosshairHit)
	{
		// Tentative beam location - still need to trace from gun
		OutBeamLocation = CrosshairHitResult.Location;
	}
	else // no crosshair trace hit
	{
		// OutBeamLocation is the End location for the line trace
	}

	// Perform a second trace, this time from the gun barrel
	FHitResult WeaponTraceHit;
	const FVector WeaponTraceStart{MuzzleSocketLocation};
	const FVector StartToEnd = OutBeamLocation - MuzzleSocketLocation;
	const FVector WeaponTraceEnd{MuzzleSocketLocation + StartToEnd * 1.25f};

	GetWorld()->LineTraceSingleByChannel(WeaponTraceHit, WeaponTraceStart, WeaponTraceEnd,
	                                     ECollisionChannel::ECC_Visibility);

	if (WeaponTraceHit.bBlockingHit) // Object between barrel and BeamEndPoint?
	{
		OutBeamLocation = WeaponTraceHit.Location;
		return true;
	}

	return false;
}

void AShooterCharacter::AimingButtonPressed()
{
	bAiming = true;
}

void AShooterCharacter::AimingButtonReleased()
{
	bAiming = false;
}

void AShooterCharacter::SetCameraFOV(float DeltaTime)
{
	if (bAiming)
	{
		//Interpolate to zoomed FOV
		CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV, CameraZoomedFOV, DeltaTime, InterpSpeed);
	}
	else
	{
		//Interpolate to default FOV
		CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV, CameraDefaultFOV, DeltaTime, InterpSpeed);
	}

	GetFollowCamera()->SetFieldOfView(CameraCurrentFOV);
}

void AShooterCharacter::SetLookRates()
{
	if (bAiming)
	{
		BaseTurnRate = AimingTurnRate;
		BaseLookUpRate = AimingLookUpRate;
	}
	else
	{
		BaseTurnRate = HipTurnRate;
		BaseLookUpRate = HipLookUpRate;
	}
}

void AShooterCharacter::CalculateCrosshairSpread(float DeltaTime)
{
	FVector2D WalkSpeedRange{0.f, 600.f};
	FVector2D VelocityMultiplierRange{0.f, 1.f};
	FVector Velocity{GetVelocity()};
	Velocity.Z = 0.f;

	// Calculate crosshair velocity factor
	CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(
		WalkSpeedRange,
		VelocityMultiplierRange,
		Velocity.Size());

	// Calculate crosshair in air factor
	if (GetCharacterMovement()->IsFalling()) // Character is in air
	{
		//Spread the crosshairs slowly while in air
		CrosshairInAirFactor = FMath::FInterpTo(
			CrosshairInAirFactor,
			2.25f,
			DeltaTime,
			2.25f);
	}
	else // Character is on the ground
	{
		// Shrink the crosshair rapidly while on the ground
		CrosshairInAirFactor = FMath::FInterpTo(
			CrosshairInAirFactor,
			0.f,
			DeltaTime,
			30.f);
	}

	// Calculate crosshair aim factor
	if (bAiming) //Are we aiming?
	{
		// Shrink crosshairs rapidly while aiming
		CrosshairAimFactor = FMath::FInterpTo(
			CrosshairAimFactor,
			.6f,
			DeltaTime,
			30.f);
	}
	else // Not aiming
	{
		// Spread crosshairs back to normal very quickly
		CrosshairAimFactor = FMath::FInterpTo(
			CrosshairAimFactor,
			0.f,
			DeltaTime,
			30.f);
	}

	if (bFiringBullet) // Are we firing bullet?
	{
		// Spread crosshairs rapidly while firing
		CrosshairShootingFactor = FMath::FInterpTo(
			CrosshairShootingFactor,
			.3f,
			DeltaTime,
			60.f);
	}
	else // Not firing
	{
		// Shrink crosshairs rapidly while aiming
		CrosshairShootingFactor = FMath::FInterpTo(
			CrosshairShootingFactor,
			0.f,
			DeltaTime,
			60.f);
	}

	CrosshairSpreadMultiplier = .5f + CrosshairVelocityFactor + CrosshairInAirFactor - CrosshairAimFactor + CrosshairShootingFactor;
}


float AShooterCharacter::GetCrosshairSpreadMultiplier() const
{
	return CrosshairSpreadMultiplier;
}

void AShooterCharacter::IncrementOverlappedItemCount(int8 Amount)
{
	if (OverlappedItemCount + Amount <= 0)
	{
		OverlappedItemCount = 0;
		bShouldTraceForItems = false;
	}
	else
	{
		OverlappedItemCount += Amount;
		bShouldTraceForItems = true;
	}
}

void AShooterCharacter::StartCrosshairBulletFire()
{
	bFiringBullet = true;

	GetWorldTimerManager().SetTimer(
		CrosshairShootTimer,
		this,
		&AShooterCharacter::FinishCrosshairBulletFire,
		ShootTimeDuration);
}


void AShooterCharacter::FinishCrosshairBulletFire()
{
	bFiringBullet = false;

	GetWorldTimerManager().ClearTimer(CrosshairShootTimer);
}

void AShooterCharacter::FireWeapon()
{
	if (FireSound)
	{
		UGameplayStatics::PlaySound2D(this, FireSound);
	}

	const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName("BarrelSocket");
	if (BarrelSocket)
	{
		const FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetMesh());

		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
		}

		FVector BeamEnd;
		bool bBeamEnd = GetBeamEndLocation(SocketTransform.GetLocation(), BeamEnd);
		if (bBeamEnd)
		{
			// Spawn impact particles after updating BeamEndPoint
			if (ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, BeamEnd);
			}
		}
		
		if (BeamParticles)
		{
			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(), BeamParticles, SocketTransform);
			if (Beam)
			{
				Beam->SetVectorParameter(FName("Target"), BeamEnd);
			}
		}
		

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && HipFireMontage)
		{
			AnimInstance->Montage_Play(HipFireMontage);
			AnimInstance->Montage_JumpToSection(FName("StartFire"));
		}

		//Start bullet fire timer for crosshairs
		StartCrosshairBulletFire();
	}
}

void AShooterCharacter::FireButtonPressed()
{
	bFireButtonPressed = true;

	StartFireTimer();
}

void AShooterCharacter::FireButtonReleased()
{
	bFireButtonPressed = false;
}

void AShooterCharacter::StartFireTimer()
{
	if (bShouldFire)
	{
		FireWeapon();

		bShouldFire = false;

		GetWorldTimerManager().SetTimer(AutoFireTimer, this, &AShooterCharacter::AutoFireReset, AutomaticFireRate);
	}
}

void AShooterCharacter::AutoFireReset()
{
	bShouldFire = true;

	if (bFireButtonPressed)
	{
		StartFireTimer();
	}
}

bool AShooterCharacter::TraceUnderCrosshairs(
	FHitResult& OutHitResult,
	FVector& OutHitLocation)
{
	//Get current size of viewport
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	//Get screen space location of crosshair
	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	//CrosshairLocation.Y -= 50.f;
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	//Get world location and direction of crosshair
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection);

	if (bScreenToWorld)
	{
		// Trace from crosshair world location outward
		const FVector Start{CrosshairWorldPosition};
		const FVector End{Start + CrosshairWorldDirection * 50'000.f};
		OutHitLocation = End;
		GetWorld()->LineTraceSingleByChannel(
			OutHitResult,
			Start,
			End,
			ECC_Visibility);
		if (OutHitResult.bBlockingHit)
		{
			OutHitLocation = OutHitResult.Location;
			return true;
		}
	}
	return false;
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("Turn", this, &AShooterCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AShooterCharacter::LookUp);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("FireButton", IE_Pressed, this, &AShooterCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("FireButton", IE_Released, this, &AShooterCharacter::FireButtonReleased);
	PlayerInputComponent->BindAction("AimingButton", IE_Pressed, this, &AShooterCharacter::AimingButtonPressed);
	PlayerInputComponent->BindAction("AimingButton", IE_Released, this, &AShooterCharacter::AimingButtonReleased);
}
