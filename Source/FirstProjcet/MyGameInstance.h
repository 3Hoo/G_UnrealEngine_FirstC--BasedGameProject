// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPROJCET_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UMyGameInstance();
	
private:
	int32 IsFirst = 0;

public:
	int32 GetIsFirst();
	void SetIsFirst(int32 num);
	
};
