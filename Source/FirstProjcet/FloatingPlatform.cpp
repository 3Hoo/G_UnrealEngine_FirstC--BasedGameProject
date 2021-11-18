// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* ------- CreateDefaultSubobject ------- */
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));


	/* ------- Hierachy ------- */
	RootComponent = Mesh;


	/* ------- 각종 value 초기화 -------- */
	StartPoint = FVector(0.0f);
	EndPoint = FVector(0.0f);
	InterpSpeed = 4.0f;
	InterpTime = 1.0f;
	bInterping = true;

}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	StartPoint = GetActorLocation();
	EndPoint += StartPoint;

	GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);

	Distance = (EndPoint - StartPoint).Size();
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInterping) {
		FVector CurrentLocation = GetActorLocation();
		FVector Interp = FMath::VInterpTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);
		SetActorLocation(Interp);

		float DistanceTraveled = (GetActorLocation() - StartPoint).Size();
		if (Distance - DistanceTraveled <= SMALL_NUMBER) {
			ToggleInterping();

			GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);
			SwapVector(StartPoint, EndPoint);
		}
	}
	
}

void AFloatingPlatform::ToggleInterping()
{
	bInterping = !bInterping;
}

void AFloatingPlatform::SwapVector(FVector& VecOne, FVector& VecTwo)
{
	FVector Temp = VecOne;
	VecOne = VecTwo;
	VecTwo = Temp;
}
