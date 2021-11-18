// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	IsFirst = 0;
}

int32 UMyGameInstance::GetIsFirst()
{
	return IsFirst;
}

void UMyGameInstance::SetIsFirst(int num)
{
	IsFirst = num;
}