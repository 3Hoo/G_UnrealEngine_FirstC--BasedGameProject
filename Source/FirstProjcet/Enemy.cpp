// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Sound/SoundCue.h"
#include "Engine/SkeletalMeshSocket.h"
#include "AIController.h"
#include "Main.h"
#include "MainPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* ------- Sphere Components ------- */
	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatShere"));

	AgroSphere->InitSphereRadius(600.0f);
	CombatSphere->InitSphereRadius(75.0f);

	AgroSphere->SetupAttachment(GetRootComponent());
	CombatSphere->SetupAttachment(GetRootComponent());

	AgroSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	/* ------- ------ ------- */

	bOverlappingCombatSphere = false;

	Health = 75.0f;
	MaxHealth = 100.0f; 
	Damage = 10.0f;

	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatCollision->SetupAttachment(GetMesh(), FName("EnemySocket"));
	//CombatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("EnemySocket"));

	bAttacking = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_COMBATMONTAGE(TEXT("/Game/Enemy/Spider/SpiderCombatMontage.SpiderCombatMontage"));
	if (AM_COMBATMONTAGE.Succeeded())
	{
		CombatMontage = AM_COMBATMONTAGE.Object;
	}

	AttackMinTime = 0.5f;
	AttackMaxTime = 3.5f;

	EnemyMovementStatus = EEnemyMovementStatus::EMS_IDLE;

	DeathDelay = 3.0f;

	bHasValidTarget = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(GetController()); // 여기서 GetContoller() 는 AController를 반환


	/* ------- CollisionVolumn Delegate 콜리전볼륨 델리게이트 ------- */
	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapEnd);
	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);
	/* ------- ------ ------- */

	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapEnd);
	
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInterpToEnemy && CombatTarget && (Alive() == true))
	{
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed);

		SetActorRotation(InterpRotation);
	}

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	


}

/* ------- Overlap Function 오버랩 함수 ------- */
void AEnemy::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Alive())
	{
		UE_LOG(LogTemp, Warning, TEXT("OtherActor"));
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			UE_LOG(LogTemp, Warning, TEXT("Main"));
			Main->UpdateCombatTarget();
			MoveToTarget(Main);
		}
	}
}
void AEnemy::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		{
			if (Main)
			{
				bHasValidTarget = false;
				if (Main->CombatTarget == this)
				{

					//Main->SetCombatTarget(nullptr);
				}
				//Main->SetHasCombatTarget(false);

				Main->UpdateCombatTarget();

				SetEnemyMovementStatus(EEnemyMovementStatus::EMS_IDLE);
				if (AIController)
				{
					AIController->StopMovement();
				}
			}
		}
	}
}
void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Alive())
	{
		AMain* Main = Cast<AMain>(OtherActor);
		{
			if (Main)
			{
				//Main->SetCombatTarget(this);
				Main->SetHasCombatTarget(true);
				bHasValidTarget = true;

				Main->UpdateCombatTarget();

				CombatTarget = Main;
				bOverlappingCombatSphere = true;
				SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
				float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
				GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime, false);
				UE_LOG(LogTemp, Warning, TEXT("Timer Start!"));
				
			}
		}
	}
}
void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor && OtherComp)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		{
			if (Main)
			{
				bOverlappingCombatSphere = false;
				//if (EnemyMovementStatus == EEnemyMovementStatus::EMS_Attacking)
				//{
					MoveToTarget(Main);
					CombatTarget = nullptr;
					SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);
				//};

				if (Main->CombatTarget == this)
				{
					//Main->SetCombatTarget(nullptr);
					Main->bHasCombatTarget = false;
					Main->UpdateCombatTarget();
				}

				if (Main->MainPlayerController)
				{
					USkeletalMeshComponent* MainMesh = Cast<USkeletalMeshComponent>(OtherComp);
					if (MainMesh)
					{
						Main->MainPlayerController->RemoveEnemyHealthBar();
					}
				}
				

				GetWorldTimerManager().ClearTimer(AttackTimer);
			}
		}
	}
}
/* ------- ------ ------- */


/* ------- Move Function 이동 함수 ------- */
void AEnemy::MoveToTarget(AMain* Target)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);

	if (AIController)
	{
		// https://docs.unrealengine.com/en-US/API/Runtime/AIModule/AAIController/MoveTo/index.html
		// https://docs.unrealengine.com/en-US/API/Runtime/AIModule/FAIMoveRequest/index.html
		// https://docs.unrealengine.com/en-US/API/Runtime/Engine/AI/Navigation/FNavPathSharedPtr/index.html
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(10.0f);

		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);

		/*
		auto PathPoints = NavPath->GetPathPoints();
		for (auto Point : PathPoints)
		{
			FVector Location = Point.Location;
			UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.0f, 8, FLinearColor::Green, 10.0f, 0.5f);
		}
		*/
	}
}
/* ------- ------ ------- */

void AEnemy::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main && bCanAttack)
		{
			bCanAttack = false;
			if (Main->HitParticles)
			{
				const USkeletalMeshSocket* TipSocket = GetMesh()->GetSocketByName("TipSocket");
				if (TipSocket)
				{
					FVector SocketLocation = TipSocket->GetSocketLocation(GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Main->HitParticles, SocketLocation, FRotator(0.0f), true);
				}


			}
			if (Main->HitSound)
			{
				UGameplayStatics::PlaySound2D(this, Main->HitSound);
			}
			if (SwingSound)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), SwingSound, 2.0f);
			}
			if (DamageTypeClass)
			{
				UGameplayStatics::ApplyDamage(Main, Damage, AIController, this, DamageTypeClass);
			}
		}
	}
}

void AEnemy::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bCanAttack = true;
}

void AEnemy::ActiveCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemy::DeactiveCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::Attack()
{
	if (Alive() && bHasValidTarget)
	{
		if (AIController)
		{
			AIController->StopMovement();
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
		}
		if (!bAttacking)
		{
			bAttacking = true;
			SetInterpToEnemy(true);
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->Montage_Play(CombatMontage, 1.35f);
				AnimInstance->Montage_JumpToSection(FName("Attack"), CombatMontage);
			}

		}
	}	
}

void AEnemy::AttackEnd()
{
	bAttacking = false;
	if (bOverlappingCombatSphere)
	{
		float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
		GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime, false);
		UE_LOG(LogTemp, Warning, TEXT("Timer Start!"));
	}
	else
	{
		SetEnemyMovementStatus(EEnemyMovementStatus::EMS_IDLE);
	}
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (Health - DamageAmount <= 0.0f)
	{
		Die(DamageCauser);
		Health -= DamageAmount;
	}
	else
	{
		Health -= DamageAmount;
	}

	return DamageAmount;
}

void AEnemy::Die(AActor* Causer)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();


	if (AnimInstance)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.00f);
		AnimInstance->Montage_JumpToSection(FName("Death"), CombatMontage);
		
	}

	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead);
	IsDead = true;
	bAttacking = false;

	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AMain* Main = Cast<AMain>(Causer);
	if (Main)
	{
		Main->UpdateCombatTarget();
	}
}

void AEnemy::DeathEnd()
{
	//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//if (AnimInstance)
	//{
		GetMesh()->bPauseAnims = 1;
		GetMesh()->bNoSkeletonUpdate = 1;
		//AnimInstance->Montage_Stop(0.5f, CombatMontage);
	//}
	
		GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::Disappear, DeathDelay);
}

bool AEnemy::Alive()
{
	
	return GetEnemyMovementStatus() != EEnemyMovementStatus::EMS_Dead;
}

FRotator AEnemy::GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.0f, LookAtRotation.Yaw, 0.0f);
	return LookAtRotationYaw;
}

void AEnemy::SetInterpToEnemy(bool Interp)
{
	bInterpToEnemy = Interp;
}

void AEnemy::Disappear()
{
	Destroy();
}