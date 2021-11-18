// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FirstProjcet.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPROJCET_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	// AnimInstance에서 BeginPlay() 를 담당하는 함수
	virtual void NativeInitializeAnimation() override;

public:

	UFUNCTION(BlueprintCallable, Category = Animation)
	void UpdateAnimationProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class AEnemy* Enemy;
	
};
