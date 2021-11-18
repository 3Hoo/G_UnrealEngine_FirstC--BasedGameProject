// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "Main.h"
#include "MainPlayerController.h"


bool AMyGameMode::GetIsFirst()
{
	UE_LOG(LogTemp, Warning, TEXT("GetIsFirst()"));
	return bIsFirst;
}
void AMyGameMode::SetIsFirst(bool B)
{
	UE_LOG(LogTemp, Warning, TEXT("SetIsFirst()"));
	bIsFirst = B;
}