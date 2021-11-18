// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolumn.h"
#include "Kismet/KismetMathLibrary.h"
#include "Critter.h"
#include "Enemy.h"
#include "AIController.h"

// Sets default values
ASpawnVolumn::ASpawnVolumn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* ------- CreateDefaultSubobject ------- */
	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));

	
	/* ------- SpawningBox Setup ------- */


}

// Called when the game starts or when spawned
void ASpawnVolumn::BeginPlay()
{
	Super::BeginPlay();
	
	if (Actor_1 && Actor_2 && Actor_3 && Actor_4)
	{
		SpawnArray.Add(Actor_1);
		SpawnArray.Add(Actor_2);
		SpawnArray.Add(Actor_3);
		SpawnArray.Add(Actor_4);
	}
}

// Called every frame
void ASpawnVolumn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolumn::GetSpawnPoint()
{
	FVector Origin = SpawningBox->GetComponentLocation();
	FVector Extent = SpawningBox->GetScaledBoxExtent();

	FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
	
	return Point;
}

TSubclassOf<AActor> ASpawnVolumn::GetSpawnActor()
{
	if (SpawnArray.Num() > 0)
	{
		int32 selection = FMath::RandRange(0, SpawnArray.Num() - 1);

		return SpawnArray[selection];
	}
	else
	{
		return nullptr;
	}
}

void ASpawnVolumn::SpawnOurActor_Implementation(UClass* ToSpawn, const FVector& Location)
{
	if (ToSpawn) {
		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;

		if (World) {
			AActor* Actor = World->SpawnActor<AActor>(ToSpawn, Location, FRotator(0.0f), SpawnParams);

			AEnemy* Enemy = Cast<AEnemy>(Actor);
			if (Enemy)
			{
				Enemy->SpawnDefaultController();

				AAIController* AICont = Cast<AAIController>(Enemy->GetController());
				if (AICont)
				{
					Enemy->AIController = AICont;
				}
			}
		}
	}
}

