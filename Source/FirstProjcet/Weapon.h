// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FirstProjcet.h"
#include "Item.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState :uint8
{
	EWS_PickUp		UMETA(DisplayName = "PickUp"),
	EWS_Equipped	UMETA(DisplayName = "Equipped"),
	EWS_MAX			UMETA(DisplayName = "MAX")
};

/**
 * 
 */
UCLASS()
class FIRSTPROJCET_API AWeapon : public AItem
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void PostInitializeComponents() override;

	AWeapon();

	/* ------- Weapon Name(Key) ------- */
	UPROPERTY(EditDefaultsOnly, Category = "SavedData")
	FString Name;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	EWeaponState WeaponState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particle")
	bool bWepaonParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkeletalMesh")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Combat")
	UBoxComponent* CombatCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	class USoundCue* OnEquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	USoundCue* SwingSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Combat")
	float Damage;

public:

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	void Equip(class AMain* Character);

	UFUNCTION()
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void ActiveCollision();
	UFUNCTION(BlueprintCallable)
	void DeactiveCollision();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	AController* WeaponInstigator;
	
	FORCEINLINE void SetWeaponInstigator(AController* Inst) { WeaponInstigator = Inst; }

	FORCEINLINE void SetWeaponState(EWeaponState State) { WeaponState = State; }
	FORCEINLINE EWeaponState GetWeaponState() { return WeaponState; }
};
