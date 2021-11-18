// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Main.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDOverlayAsset) {
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}
	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

	if (WEnemyHealthBar)
	{
		EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);
		if (EnemyHealthBar)
		{
			EnemyHealthBar->AddToViewport();
			EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
		}
		FVector2D Vec(0.0f, 0.0f);
		EnemyHealthBar->SetAlignmentInViewport(Vec);
	}

	if (WPauseMenu)
	{
		PauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);
		if (PauseMenu)
		{
			PauseMenu->AddToViewport();
			PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyHealthBar)
	{
		FVector2D PositionInViewPort;
		ProjectWorldLocationToScreen(EnemyLocation, PositionInViewPort);
		PositionInViewPort.Y -= 80.0f;

		FVector2D SizeInViewPort = FVector2D (300.0f, 25.0f);

		EnemyHealthBar->SetPositionInViewport(PositionInViewPort);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewPort);
	}
}

void AMainPlayerController::DisplayPauseMenu_Implementation()
{
	if (PauseMenu)
	{
		bPauseMenuVisible = true;
		PauseMenu->SetVisibility(ESlateVisibility::Visible);

		FInputModeUIOnly InputModeUIOnly;
		FInputModeGameAndUI InputModeGameAndUI;

		SetInputMode(InputModeGameAndUI);
		bShowMouseCursor = true;
	}
}
void AMainPlayerController::RemovePauseMenu_Implementation()
{
	if (PauseMenu)
	{
		GameModeOnly();

		bPauseMenuVisible = false;
		//PauseMenu->SetVisibility(ESlateVisibility::Hidden);  => 블루프린트에서 작성하는 것으로 변경
		bShowMouseCursor = false;
	}
}

void AMainPlayerController::TogglePauseMenu()
{
	if (bPauseMenuVisible)
	{
		RemovePauseMenu();
	}
	else
	{
		DisplayPauseMenu();
	}
}

void AMainPlayerController::DisplayEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
}
void AMainPlayerController::RemoveEnemyHealthBar()
{
	if (EnemyHealthBar)
	{	
		bEnemyHealthBarVisible = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::GameModeOnly()
{
	FInputModeGameOnly InputModeGameOnly;

	SetInputMode(InputModeGameOnly);
}

