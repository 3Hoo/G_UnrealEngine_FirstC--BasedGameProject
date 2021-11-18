// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPROJCET_API AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	bool GetIsFirst();
	void SetIsFirst(bool B);

private:
	bool bIsFirst = true;
};
