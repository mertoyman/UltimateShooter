// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BulletHitInterface.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"
#include "Enemy.generated.h"

UCLASS()
class ULTIMATESHOOTER_API AEnemy : public ACharacter, public IBulletHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void ShowHealthBar();

	UFUNCTION(BlueprintImplementableEvent)
	void HideHealthBar();

	void Die();

	void PlayHitMontage(FName Section, float PlayRate = 1.f);

	void ResetHitReactTimer();

	UFUNCTION(BlueprintCallable)
	void StoreHitNumber(UUserWidget* HitNumber, FVector Location);

	UFUNCTION()
	void DestroyHitNumber(UUserWidget* HitNumber);

	void UpdateHitNumbers();
	
	UFUNCTION()
	void AgroSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void SetStunned(bool Stunned);
	
	UFUNCTION()
	void CombatRangeOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
	UFUNCTION()
	void CombatRangeEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void PlayAttackMontage(FName Section, float PlayRate = 1.f);

	UFUNCTION(BlueprintPure)
	FName GetAttackSectionName();


	UFUNCTION()
	void LeftWeaponOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void RightWeaponOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void ActivateLeftWeapon();

	UFUNCTION(BlueprintCallable)
	void DeactivateLeftWeapon();
	
	UFUNCTION(BlueprintCallable)
	void ActivateRightWeapon();

	UFUNCTION(BlueprintCallable)
	void DeactivateRightWeapon();
	
	void DoDamage(class AShooterCharacter* TargetActor);

	void SpawnBloodParticles(AShooterCharacter* Character, FName WeaponSocketName);

	// Attempt to stun character
	void StunCharacter(AShooterCharacter* Character);

	void ResetCanAttack();
	
	void PlayDeathMontage(FName Section, float PlayRate = 1.f);

	UFUNCTION(BlueprintCallable)
	void FinishDeath();

	void DestroyEnemy();
	
private:
	/* Particle effect to spawn when bullet impacts */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	class UParticleSystem* ImpactParticle;

	/* Sound to play when bullet impacts */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	USoundCue* ImpactSound;

	/* Current health of the enemy */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Combat, meta= (AllowPrivateAccess = true))
	float Health;

	/* Maximum health of the enemy */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	float MaxHealth;

	/* Name of the head bone */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	FString HeadBone;

	/* Time to display health bar once shot */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	float HealthBarDisplayTime;
	
	FTimerHandle HealthBarTimer;

	/* Montage containing Hit and Death animations */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	UAnimMontage* HitMontage;

	FTimerHandle HitReactTimer;

	/* Min time of delay between hit reactions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	float HitReactDelayMin;

	/* Max time of delay between hit reactions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	float HitReactDelayMax;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Combat, meta= (AllowPrivateAccess = true))
	bool bCanHitReact;

	/* Map to store  HitNumber widgets and their hit locations */
	UPROPERTY(VisibleAnywhere, Category=Combat, meta= (AllowPrivateAccess = true))
	TMap<UUserWidget*, FVector> HitNumbers;

	/* Time before a HitNumber is removed from the screen */
	UPROPERTY(EditAnywhere, Category=Combat, meta= (AllowPrivateAccess = true))
	float HitNumberDestroyTime;

	/* Behavior tree for the AI character */
	UPROPERTY(EditAnywhere, Category="Behavior Tree", meta=(AllowPrivateAccess = true))
	class UBehaviorTree* BehaviorTree;

	/* Point for the enemy to move to */
	UPROPERTY(EditAnywhere, Category="Behavior Tree", meta=(AllowPrivateAccess = true, MakeEditWidget = true))
	FVector PatrolPoint;

	/* Point for the enemy to move to */
	UPROPERTY(EditAnywhere, Category="Behavior Tree", meta=(AllowPrivateAccess = true, MakeEditWidget = true))
	FVector PatrolPoint2;

	class AEnemyController* EnemyController;

	/* Overlap sphere for when the enemy becomes hostile */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta=(AllowPrivateAccess = true))
	class USphereComponent* AgroSphere;

	/* True when playing the get hit animation */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= Combat, meta=(AllowPrivateAccess = true))
	bool bStunned;

	/* Chance of being stunned. 0: no stun chance, 1: 100% stun chance  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta=(AllowPrivateAccess = true))
	float StunChance;

	/* True when in attack range; time to attack! */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= Combat, meta=(AllowPrivateAccess = true))
	bool bInAttackRange;

	/* Overlap sphere for attack range */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta=(AllowPrivateAccess = true))
	USphereComponent* CombatRangeSphere;

	/* Montage containing different attack animations */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	/* The four attack montage section names */
	FName AttackLFast;
	FName AttackRFast;
	FName AttackL;
	FName AttackR;

	/* Collision volume for the left weapon */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	class UBoxComponent* LeftWeaponCollision;

	/* Collision volume for the right weapon */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	UBoxComponent* RightWeaponCollision;

	/* Base damage for enemy */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	FName LeftWeaponSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	FName RightWeaponSocket;

	/* True when enemy can attack */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	bool bCanAttack;

	FTimerHandle AttackWaitTimer;

	/* Minimum wait time between attacks */ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	float AttackWaitTime;

	/* Montage containing different death animations */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	UAnimMontage* DeathMontage;

	bool bDying;

	FTimerHandle DeathTimer;

	/* Time after death */ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat, meta= (AllowPrivateAccess = true))
	float DeathTime;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BulletHit_Implementation(FHitResult HitResult, AActor* Shooter, AController* Instigator) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FORCEINLINE FString GetHeadBone() const { return HeadBone; }

	UFUNCTION(BlueprintImplementableEvent)
	void ShowHitNumber(int32 Damage, FVector HitLocation, bool bHeadshot);

	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
};
