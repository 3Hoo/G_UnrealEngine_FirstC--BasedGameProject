// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FirstProjcet.h"
#include "GameFramework/Actor.h"
#include "FloorSwitch.generated.h"

UCLASS()
class FIRSTPROJCET_API AFloorSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorSwitch();

	// Overlap Volumn for functionality to be triggered
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Floor Switch")
	UBoxComponent* TriggerBox;

	// Switch for the character to step on
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Floor Switch")
	UStaticMeshComponent* FloorSwitch;

	// Door to move 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Floor Switch")
	UStaticMeshComponent* Door;

	// Door 의 초기 위치
	UPROPERTY(BlueprintReadWrite, Category = "Floor Switch")
	FVector InitialDoorLocation;

	// Switch 의 초기 위치
	UPROPERTY(BlueprintReadWrite, Category = "Floor Switch")
	FVector InitialSwitchLocation;

	// Timer
	FTimerHandle SwitchHandle;

	// 문이 몇 초 뒤에 닫힐 것인지
	UPROPERTY(EditAnywhere, Category = "Floor Switch")
	float SwitchTime;

	// 케릭터가 스위치 위에 있니?
	bool bCharacterOnSwitch;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
	void RaiseDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
	void LowerDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
	void RaiseFloorSwitch();

	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
	void LowerFloorSwitch();

	UFUNCTION(BlueprintCallable, Category = "Floor Switch")
	void UpdateDoorLocation(float Z);

	UFUNCTION(BlueprintCallable, Category = "Floor Switch")
	void UpdateFloorSwitchLocation(float Z);

	void CloseDoor();
};
