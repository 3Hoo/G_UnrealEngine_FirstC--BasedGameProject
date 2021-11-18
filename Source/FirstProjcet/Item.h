// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FirstProjcet.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class FIRSTPROJCET_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	// Base Shape Collision
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Collision")
	USphereComponent* CollisionVolumn;

	// Base MeshComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Mesh")
	UStaticMeshComponent* Mesh;

	// Base Idle Particle System
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
	UParticleSystemComponent* IdleParticleComponent;

	// Base Overlap Particle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
	UParticleSystem* OverlapParticle;

	// Base Sound Cue
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	class USoundCue* OverlapSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	float SoundSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | ItemProperties")
	bool bRotate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | ItemProperties")
	float RotationRate;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
