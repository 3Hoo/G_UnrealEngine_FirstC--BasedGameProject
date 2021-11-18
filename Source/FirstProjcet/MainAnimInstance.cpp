// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "Main.h"

void UMainAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (Pawn == nullptr) 
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			Main = Cast<AMain>(Pawn);
		}
	}
}

void UMainAnimInstance::UpdateAnimationProperties()
{
	// 만약 Pawn이 없으면, 가져온다
	if (Pawn == nullptr) 
	{
		Pawn = TryGetPawnOwner();
	}

	// Pawn이 유효하면, 
	if (Pawn) 
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0);
		MovementSpeed = LateralSpeed.Size();

		bIsInAir = Pawn->GetMovementComponent()->IsFalling();

		if (Main == nullptr)
		{
			Main = Cast<AMain>(Pawn);
		}
	}
}
