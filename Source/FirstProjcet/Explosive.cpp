// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosive.h"
#include "Sound/SoundCue.h"
#include "Main.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"

AExplosive::AExplosive()
{
	SoundSize = 0.1f;
	Damage = 15.0f;
}


void AExplosive::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	UE_LOG(LogTemp, Warning, TEXT("Explosive::On Overlap Begin()"));
	
	if (OtherActor) 
	{
		AMain* Main = Cast<AMain>(OtherActor);
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Main || Enemy)
		{
			if (OverlapParticle) {
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticle, GetActorLocation(), FRotator(0.0f), true);
			}

			if (OverlapSound) {
				UGameplayStatics::PlaySound2D(this, OverlapSound, SoundSize);
			}
			
			UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, DamageTypeClass);

			Destroy();
		}
	}
}
void AExplosive::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	UE_LOG(LogTemp, Warning, TEXT("Explosive::On Overlap End()"));
}
