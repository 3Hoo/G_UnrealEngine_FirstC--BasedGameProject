// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FirstProjcet.h"
#include "GameFramework/SaveGame.h"
#include "FirstSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStats
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float Health;// = 100.0f;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float MaxHealth;// = 200.0f;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float Stamina;// = 15.0f;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float MaxStamina;// = 50.0f;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	int32 Coins;

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FVector Location;// = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FRotator Rotation;// = FRotator(0.0f, 0.0f, 0.0f);

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FString WeaponName = TEXT("");

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FString LevelName = TEXT("");

};

/**
 * 
 */
UCLASS()
class FIRSTPROJCET_API UFirstSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UFirstSaveGame();
	
	UPROPERTY(VisibleAnywhere, Category = "Basic")
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	uint32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	FCharacterStats CharacterStats;

	
};
