// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp.h"
#include "Main.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

APickUp::APickUp()
{

}


void APickUp::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	
	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			OnPickUpBP(Main);
			Main->PickupLocations.Add(GetActorLocation());

			if (OverlapParticle) {
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticle, GetActorLocation(), FRotator(0.0f), true);
			}

			if (OverlapSound) {
				UGameplayStatics::PlaySound2D(this, OverlapSound, SoundSize);
			}

			Destroy();
		}
	}

}
void APickUp::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
