// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FirstProjcet.h"

#include "GameFramework/Actor.h"
#include "SpawnVolumn.generated.h"

UCLASS()
class FIRSTPROJCET_API ASpawnVolumn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolumn();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawningBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<AActor> Actor_1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<AActor> Actor_2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<AActor> Actor_3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<AActor> Actor_4;

	TArray<TSubclassOf<AActor>> SpawnArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
	FVector GetSpawnPoint();

	UFUNCTION(BlueprintPure)
	TSubclassOf<AActor> GetSpawnActor();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Spawning")
	void SpawnOurActor(UClass* ToSpawn, const FVector& Location);

};
