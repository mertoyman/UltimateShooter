// Fill out your copyright notice in the Description page of Project Settings.


#include "UltimateShooterAnimInstance.h"

#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet\KismetMathLibrary.h>

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

		//FString RotationMsg = FString::Printf(TEXT("Base Aim Rotation: %f"), AimRotation.Yaw);
		//FString MovementRotationMsg = FString::Printf(TEXT("Movement Rotation: %f"), MovementRotation.Yaw);
		//FString OffsetMsg = FString::Printf(TEXT("Movement Offset Yaw: %f"), MovementOffsetYaw);

		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 0, FColor::White, OffsetMsg);
		}*/
	}
}

void UUltimateShooterAnimInstance::NativeInitializeAnimation()
{
	ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
}
