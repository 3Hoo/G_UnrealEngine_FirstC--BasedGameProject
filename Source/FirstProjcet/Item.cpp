// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Sound/SoundCue.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* ------- CreateDefaultSubobject ------- */
	CollisionVolumn = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolumn"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	IdleParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticleComponent"));


	/* ------- Hierachy ------- */
	RootComponent = CollisionVolumn;
	Mesh->SetupAttachment(RootComponent);
	IdleParticleComponent->SetupAttachment(RootComponent);


	/* ------- CollisionVolumn Delegate ------- */
	CollisionVolumn->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	CollisionVolumn->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);

	/**/
	SoundSize = 1.0f;
	bRotate = false;
	RotationRate = 45.0f;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRotate) {
		FRotator Rotation = GetActorRotation();
		Rotation.Yaw += DeltaTime * RotationRate;
		SetActorRotation(Rotation);
	}
}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	
	
}
void AItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}
