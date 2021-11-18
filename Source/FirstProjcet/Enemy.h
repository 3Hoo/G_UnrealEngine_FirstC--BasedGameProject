// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FirstProjcet.h" 
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyMovementStatus :uint8
{
	EMS_IDLE			UMETA(DisplayName = "Idle"),
	EMS_MoveToTarget	UMETA(DisplayName = "MoveToTarget"),
	EMS_Attacking		UMETA(DisplayName = "Attacking"),
	EMS_Dead			UMETA(DisplayName = "Dead"),

	EMS_MAX				UMETA(DisplayName = "DefaultMax")
};

UCLASS(BlueprintType)
class FIRSTPROJCET_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	/* ------- Animation / Movement 관련 ------- */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	EEnemyMovementStatus EnemyMovementStatus;

	FORCEINLINE void SetEnemyMovementStatus(EEnemyMovementStatus Status) { EnemyMovementStatus = Status; }
	FORCEINLINE EEnemyMovementStatus GetEnemyMovementStatus() { return EnemyMovementStatus; }


	/* ------- Sphere Components ------- */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	USphereComponent* AgroSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	USphereComponent* CombatSphere;



	/* ------- AI ------- */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController* AIController;


	/* ------- Combat ------- */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class USoundCue* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	USoundCue* SwingSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	UBoxComponent* CombatCollision;

	FTimerHandle AttackTimer;

	FTimerHandle DeathTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float DeathDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackMinTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackMaxTime;

	bool bHasValidTarget;

	// Main를 바라보도록 회전하기
	float InterpSpeed = 15.0f;
	bool bInterpToEnemy = false;
	void SetInterpToEnemy(bool Interp);



	FRotator GetLookAtRotationYaw(FVector Target);

	FORCEINLINE void SetCombatTarget(AMain* Target) { CombatTarget = Target; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* ------- Overlap Function 오버랩 함수 ------- */
	UFUNCTION()
	virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	/* ------- ------ ------- */


	/* ------- Move Function / Variable 이동 함수 / 변수 ------- */
	UFUNCTION(BlueprintCallable)
	void MoveToTarget(class AMain* Target);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	bool bOverlappingCombatSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	AMain* CombatTarget;
	/* ------- ------ ------- */

	UFUNCTION()
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void ActiveCollision();
	UFUNCTION(BlueprintCallable)
	void DeactiveCollision();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* CombatMontage;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bAttacking;

	UFUNCTION(BlueprintCallable)
	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	bool bCanAttack = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Die(AActor* Causer);

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	UFUNCTION(BlueprintCallable)
	bool Alive();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsDead = false;

	void Disappear();
};
