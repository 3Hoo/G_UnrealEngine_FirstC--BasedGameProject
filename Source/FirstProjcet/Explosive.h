// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FirstProjcet.h"
#include "Item.h"
#include "Explosive.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPROJCET_API AExplosive : public AItem
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", Meta = (AllowPrivateAccess = true))
	float Damage;
	
public:
	AExplosive();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
};
