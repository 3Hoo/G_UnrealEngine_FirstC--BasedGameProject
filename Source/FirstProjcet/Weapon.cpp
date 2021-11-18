// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Main.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"

void AWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

AWeapon::AWeapon()
{
	/* ------- CreateDefaultSubobject ------- */
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));

	/* ------- Hierachy ------- */
	SkeletalMesh->SetupAttachment(GetRootComponent());
	CombatCollision->SetupAttachment(GetRootComponent());

	

	bWepaonParticle = true;

	WeaponState = EWeaponState::EWS_PickUp;

	Damage = 25.0f;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (bWepaonParticle)
	{
		IdleParticleComponent->Activate();
	}
	/* ------- CombatCollision Collision Setting -------- */
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapEnd);
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (WeaponState == EWeaponState::EWS_PickUp && OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			Main->SetActiveOverlappingItem(this);
		}
	}
}

void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			Main->SetActiveOverlappingItem(nullptr);
		}
	}

}

void AWeapon::Equip(AMain* Character)
{
	if (Character)
	{
		SetWeaponInstigator(Character->GetController());

		// 카메라와 케릭터와 충돌 무시하기
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		// 물리 시뮬레이션 안하게 하기
		SkeletalMesh->SetSimulatePhysics(false);

		const USkeletalMeshSocket* RightHandSocket = Character->GetMesh()->GetSocketByName("RightHandSocket");
		if (RightHandSocket)
		{
			RightHandSocket->AttachActor(this, Character->GetMesh());
			bRotate = false;
			
			Character->SetEquippedWeapon(this);
			Character->SetActiveOverlappingItem(nullptr);
			if (OnEquipSound && (WeaponState == EWeaponState::EWS_PickUp))
			{
				UGameplayStatics::PlaySound2D(this, OnEquipSound);
			}
			WeaponState = EWeaponState::EWS_Equipped;
		}

		if (bWepaonParticle)
		{
			IdleParticleComponent->Deactivate();
			bWepaonParticle = false;
		}

	}
}


void AWeapon::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Enemy)
		{
			if (Enemy->HitParticles)
			{
				const USkeletalMeshSocket* WeaponSocket = SkeletalMesh->GetSocketByName(TEXT("WeaponSocket"));
				if (WeaponSocket)
				{
					FVector SocketLocation = WeaponSocket->GetSocketLocation(SkeletalMesh);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->HitParticles, SocketLocation, FRotator(0.0f), true);
				}
				

			}
			if (Enemy->HitSound)
			{
				UGameplayStatics::PlaySound2D(this, Enemy->HitSound);
			}
			if (DamageTypeClass)
			{
				UGameplayStatics::ApplyDamage(Enemy, Damage, WeaponInstigator, this, DamageTypeClass);
			}
		}
	}
}

void AWeapon::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}


void AWeapon::ActiveCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeapon::DeactiveCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
