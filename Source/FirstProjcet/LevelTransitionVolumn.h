// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FirstProjcet.h"
#include "GameFramework/Actor.h"
#include "LevelTransitionVolumn.generated.h"

UCLASS()
class FIRSTPROJCET_API ALevelTransitionVolumn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelTransitionVolumn();

	/* ------- Basic Components ------- */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Transition")
	UBoxComponent* TransitionVolumn;


	//UBillboardComponent* BillBoard;


	/* ------- Components for "Overlap" ------- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transition")
	FName TransitionLevelName;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* ------- Functions for "Overlap" ------- */
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
