// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FirstProjcet.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

UENUM(BlueprintType) // ���� �������Ʈ���� ������ ������ �� �� enum�� Ÿ������ ������ �� �ִ�
enum class EMovementStatus : uint8
{
	// EnumMovementStatus_...
	EMS_Normal		UMETA(DisplayName = "Normal"),
	EMS_Sprinting	UMETA(DisplayName = "Sprinting"),
	EMS_Dead		UMETA(DisplayName = "Dead"),

	EMS_MAX			UMETA(DisplayName = "DefaultMAX")

};

UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	ESS_Normal UMETA(DisplayName = "Normal"),
	ESS_BelowMinimum UMETA(DisplayName = "BelowMinimum"),
	ESS_Exhausted UMETA(DisplayName = "Exhausted"),
	ESS_ExhaustedRecovering UMETA(DisplayName = "ExhaustedRecovering"),
	ESS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class FIRSTPROJCET_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

	/* ------- Weapon Storage ���� ���� ------- */
	UPROPERTY(EditDefaultsOnly, Category = "SavedData")
	TSubclassOf<class AItemStorage> WeaponStorage;


	/* ------- Enemy HUD ------- */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	class AMainPlayerController* MainPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	FVector CombatTargetLocation;


	/* ------- GameMode ------- */
	UPROPERTY(Visibleanywhere, BlueprintReadOnly, Category = "Controller")
	class AMyGameMode* MyGameMode;
	

	/* ------ Weapon, Item ------- */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
	class AWeapon* EquippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
	class AItem* ActiveOverlappingItem;

	void SetEquippedWeapon(AWeapon* WeaponToSet);
	FORCEINLINE AWeapon* GetEquippedWeapon() { return EquippedWeapon; }
	FORCEINLINE void SetActiveOverlappingItem(AItem* Item = nullptr) { ActiveOverlappingItem = Item; }
	/* ------- */

	
	/* ------- Pickup Debuging ------- */
	TArray<FVector> PickupLocations;

	UFUNCTION(BlueprintCallable)
	void ShowPickupLoations();
	/* ------- --------------- ------- */


	/* ------- Stamina ���� ���� * �Լ� ------- */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EStaminaStatus StaminaStatus;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float StaminaDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MinSprintStamina;

	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; }
	/*------- ------------------------- ------- */


	/* ------- Movement ���� ���� + �Լ� ------- */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EMovementStatus MovementStatus;

	// Set MovementStatus
	void SetMovementStatus(EMovementStatus Status);

	// Default Running speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float RunningSpeed;
	
	// Default Sprint Speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float SprintingSpeed;

	// Sprinting�� �ϴ��� ���ϴ��� Ȯ��
	bool bShiftKeyDown;

	// Pressed to enable Sprinting 
	void ShiftKeyDown();

	// Released to stop
	void ShiftKeyUp();

	bool bMovingKeyDown;

	void MovingKeyDown();

	void MovingKeyUp();

	// Enemy�� �ٶ󺸵��� ȸ���ϱ�
	float InterpSpeed = 15.0f;
	bool bInterpToEnemy = false;
	void SetInterpToEnemy(bool Interp);


	/* ------- Combat ------- */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class USoundCue* HitSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	class AEnemy* CombatTarget = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bHasCombatTarget;

	FRotator GetLookAtRotationYaw(FVector Target);

	FORCEINLINE void SetCombatTarget(AEnemy* Target) { CombatTarget = Target; bHasCombatTarget = true; }
	FORCEINLINE void SetHasCombatTarget(bool HasTarget) { bHasCombatTarget = HasTarget; }

	/* ------- --------------------------- ------- */


	// �÷��̾ ������� ī�޶�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	// CameraBoom Positioning the Camera behind the player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	// ī�޶� ��/�� ȸ�� �ӵ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	// ī�޶� ��/�� ȸ�� �ӵ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/**
	*
	* Player Stats
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	int32 Coins = 0;

	void DecrementHealth(float Amount);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Die();

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	virtual void Jump() override;

	UFUNCTION(BlueprintCallable)
	void IncrementCoins(int32 Amount);
	UFUNCTION(BlueprintCallable)
	void IncrementHealth(int32 Amount);

protected:
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;



public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called for Forward / Backward input
	void MoveForward(float value);

	// Called for Right / Left input
	void MoveRight(float value);

	bool CanMove(float Value);

	/* Called via input to turn at a given rate
	* @Param Rate This is a normalized rate, i.e. 1.0�� ���ϴ� turn rate�� 100%
	*/
	void TurnAtRate(float rate);
	void LookUpAtRate(float rate);

	void Turn(float yaw);
	void LookUp(float pitch);

	
	// ���콺 ��Ʈ�� Mouse Control
	bool bLMBDown;
	void LMBDown();
	void LMBUp();


	// PauseMenu control 
	bool bESCDown;
	void ESCDown();
	void ESCUp();


	// Getter of CameraBoom & FollwCamera
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Combat System
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	UAnimMontage* CombatMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anims")
	bool bAttacking;
	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void PlaySwingSound();

	void UpdateCombatTarget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<AEnemy> EnemyFilter;


	/* ------- Level Switch ���� ����ġ ------- */
	void SwitchLevel(FName LevelName);


	/* ------- Save Game ���� ���̺� ------- */
	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame(bool bSetPosition);

	UFUNCTION(BlueprintCallable)
	void LoadGameNoSwitch();

//protected:
	//static const int32 ChangedLevel = 0;


};
