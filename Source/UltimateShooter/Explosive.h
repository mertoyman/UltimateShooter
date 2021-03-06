// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BulletHitInterface.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "Explosive.generated.h"

UCLASS()
class ULTIMATESHOOTER_API AExplosive : public AActor, public IBulletHitInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosive();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	/* Explosion when hit by a bullet */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat", meta=(AllowPrivateAccess = true))
	class UParticleSystem* ExplodeParticle;

	/* Sound to play when bullet impacts */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	USoundCue* ImpactSound;

	/* Mesh for the explosive */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Combat, meta= (AllowPrivateAccess = true))
	class UStaticMeshComponent* ExplosiveMesh;
	
	/* Used to determine what actors overlap during explosion */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	class USphereComponent* OverlapSphere;

	/* Damage amount for explosives */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	float Damage;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BulletHit_Implementation(FHitResult HitResult, AActor* Shooter, AController* ShooterController) override;

};
