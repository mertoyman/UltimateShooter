// Fill out your copyright notice in the Description page of Project Settings.


#include "UltimateShooterAnimInstance.h"

#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet\KismetMathLibrary.h>

UUltimateShooterAnimInstance::UUltimateShooterAnimInstance() :
Speed(0.f),
bIsInAir(false),
bIsAccelerating(false),
MovementOffsetYaw(0.f),
LastMovementOffsetYaw(0.f),
bAiming(false),
CharacterYaw(0.f),
CharacterYawLastFrame(0.f),
RootYawOffset(0.f)

{
}

void UUltimateShooterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (!ShooterCharacter)
	{
		ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
	}
	else
	{
		//Get the lateral speed of the character from velocity
		FVector Velocity { ShooterCharacter->GetVelocity() };
		Velocity.Z = 0;
		Speed = Velocity.Size();

		//Is the character in the air?
		bIsInAir = ShooterCharacter->GetCharacterMovement()->IsFalling();

		//Is the character accelerating?
		bIsAccelerating = ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0 ? true : false;
		
		FRotator AimRotation = ShooterCharacter->GetBaseAimRotation();
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(ShooterCharacter->GetVelocity());
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;

		if (ShooterCharacter->GetVelocity().Size() > 0.f) 
		{
			LastMovementOffsetYaw = MovementOffsetYaw;
		}

		bAiming = ShooterCharacter->GetAiming();
	}
	TurnInPlace();
}

void UUltimateShooterAnimInstance::NativeInitializeAnimation()
{
	ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
}

void UUltimateShooterAnimInstance::TurnInPlace()
{
	if (!ShooterCharacter) return;
	if(Speed > 0 )
	{
		// Dont want to turn in place; Character moving
	}
	else
	{
		CharacterYawLastFrame = CharacterYaw;
		CharacterYaw = ShooterCharacter->GetActorRotation().Yaw;
		const float YawDelta { CharacterYaw - CharacterYawLastFrame };

		RootYawOffset -= YawDelta;

		if(GEngine) GEngine->AddOnScreenDebugMessage(
			1,
			-1,
			FColor::Blue,
			FString::Printf(TEXT("Character Yaw: %f"), CharacterYaw));

		if(GEngine) GEngine->AddOnScreenDebugMessage(
			2,
			-1,
			FColor::Red,
			FString::Printf(TEXT("RootYawOffset: %f"), RootYawOffset));
	}
	
}
