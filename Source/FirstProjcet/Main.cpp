 // Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon.h"
#include "Enemy.h"
#include "Sound/SoundCue.h"
#include "MainPlayerController.h"
#include "FirstSaveGame.h"
#include "ItemStorage.h"
#include "MyGameMode.h"
#include "MyGameInstance.h"


// Sets default values
AMain::AMain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* ------- CreateDefaultSubobject -------*/
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));


	/* ------- Hierachy ------- */
	CameraBoom->SetupAttachment(GetRootComponent());
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);


	/* ------------ CameraBoom Setup ------------ */
	CameraBoom->TargetArmLength = 600.0f;	// Camera follows at this distance			
	CameraBoom->bUsePawnControlRotation = true;		// Rotate arm based on Controller


	/* ------------ FollowCamera Setup ------------ */
	// Attach the Camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false;


	/* ------- CapsuleComponent Setup ------- */
	GetCapsuleComponent()->SetCapsuleSize(75.0f, 85.0f);


	/* ------- Camera Turn Rates Setup ------- */
	BaseTurnRate = 65.0f;
	BaseLookUpRate = 65.0f;

	/* ------- 카메라 회전에 따른 케릭터 회전 끄기 => 카메라만 회전하게 하기 -------- */
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	/* ------- Configure Character Movement -------- */
	GetCharacterMovement()->bOrientRotationToMovement = true;	// input의 direction대로 character가 moves...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 900.0f, 0.0f);	// ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 650.0f;
	GetCharacterMovement()->AirControl = 0.2f;	// 공중에서 움직이는 정도

	/* ------- Player Stats 초기화 ------- */
	MaxHealth = 200.0f;
	Health = 150.0f;
	MaxStamina = 150.0f;
	Stamina = 120.0f;
	Coins = 0;

	/* ------- Speed 변수들 초기화 ------- */
	RunningSpeed = 650.0f;
	SprintingSpeed = 950.0f;
	bShiftKeyDown = false;
	bMovingKeyDown = false;

	/* ------- Stamina 변수들 초기화 ------- */
	StaminaDrainRate = 15.0f;
	MinSprintStamina = 50.0f;

	/* ------- Enums 초기화 ------- */
	MovementStatus = EMovementStatus::EMS_Normal;
	StaminaStatus = EStaminaStatus::ESS_Normal;


	/* ------- Mouse Control ------- */
	bLMBDown = false;
	

	/* ------- ESC ------- */
	bESCDown = false;

	bHasCombatTarget = false;


}

void AMain::SetEquippedWeapon(AWeapon* WeaponToSet)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
	EquippedWeapon = WeaponToSet;
}

void AMain::PostInitializeComponents()
{
	Super::PostInitializeComponents();


}


// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();

	static int n = 0;
	UE_LOG(LogTemp, Warning, TEXT("n = %d"), n);

	MainPlayerController = Cast<AMainPlayerController>(GetController());
	MyGameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	//if (ChangedLevel > 0) // 이 if문을 통과 못하고 있다...static 변수로 해서 해결하려 했지만, ue4에서 static 쓰는 방법을 모르겠음
	FString Map = GetWorld()->GetMapName();
	Map.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	//LoadGameNoSwitch();

	/*
	if (Map != "SunTemple")
	{
		LoadGameNoSwitch();
		if (MainPlayerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("123456789"));
			MainPlayerController->GameModeOnly();
		}
	}
	*/

	
	if (n > 0)
	{
		LoadGameNoSwitch(); 
	}
			
	if (MainPlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("123456789"));
		MainPlayerController->GameModeOnly();
	}

	if (n == 0)
		n++;	// 어느정도 해결했지만, 일일히 다시 리컴파일을 해줘야 생각대로 작동한다. 게임이 종료되어도 n의 값이 1로 고정되고 있다.
				// 만약 게임을 패키징하고도 이런 일이 발생한다면, 근본적인 해결책이 새로 필요하다. 하지만, 게임을 패키징하고 게임을 종료할 때마다 n이 초기화된다면, 개꿀

	  // 딱 한번 초기화되는 GameInstace를 이용해서 해결가능? */

}

void AMain::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	float DeltaStamina = StaminaDrainRate * DeltaTime;

	if ((MovementStatus == EMovementStatus::EMS_Dead)) return;

	switch (StaminaStatus)
	{
	case EStaminaStatus::ESS_Normal : 
		if (bShiftKeyDown && bMovingKeyDown && !(this->GetCharacterMovement()->IsFalling()))
		{
			if (Stamina - DeltaStamina <= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
				Stamina -= DeltaStamina;
			}
			else 
			{
				Stamina -= DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Sprinting);
		}
		else
		{
			if (Stamina + DeltaStamina >= MaxStamina)
			{
				Stamina = MaxStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
		
	case EStaminaStatus::ESS_BelowMinimum :
		if (bShiftKeyDown && bMovingKeyDown && !(this->GetCharacterMovement()->IsFalling()))
		{
			if (Stamina - DeltaStamina <= 0.0f)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
				Stamina = 0;
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
			else
			{
				Stamina -= DeltaStamina;
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
		}
		else
		{
			if (Stamina + DeltaStamina >= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
				Stamina += DeltaStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;

	case EStaminaStatus::ESS_Exhausted :
		if (bShiftKeyDown && bMovingKeyDown && !(this->GetCharacterMovement()->IsFalling()))
		{
			Stamina = 0;
		}
		else
		{
			SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);

		break;

	case EStaminaStatus::ESS_ExhaustedRecovering :
		if (Stamina + DeltaStamina >= MinSprintStamina)
		{
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
			Stamina += DeltaStamina;
		}
		else
		{
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	default :
		;

	}

	if (bInterpToEnemy && CombatTarget)
	{
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed);

		SetActorRotation(InterpRotation);
	}

	if (CombatTarget)
	{
		CombatTargetLocation = CombatTarget->GetActorLocation();
		if (MainPlayerController)
		{
			MainPlayerController->EnemyLocation = CombatTargetLocation;
		}
	}
}

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMain::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AMain::ShiftKeyDown);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AMain::ShiftKeyUp);

	PlayerInputComponent->BindAction(TEXT("Moving"), EInputEvent::IE_Pressed, this, &AMain::MovingKeyDown);
	PlayerInputComponent->BindAction(TEXT("Moving"), EInputEvent::IE_Released, this, &AMain::MovingKeyUp);

	PlayerInputComponent->BindAction(TEXT("LMB"), EInputEvent::IE_Pressed, this, &AMain::LMBDown);
	PlayerInputComponent->BindAction(TEXT("LMB"), EInputEvent::IE_Released, this, &AMain::LMBUp);

	PlayerInputComponent->BindAction(TEXT("ESC"), EInputEvent::IE_Pressed, this, &AMain::ESCDown);
	PlayerInputComponent->BindAction(TEXT("ESC"), EInputEvent::IE_Released, this, &AMain::ESCUp);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMain::MoveRight);

	//PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMain::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMain::LookUp);

	PlayerInputComponent->BindAxis(TEXT("TurnRate"), this, &AMain::TurnAtRate);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AMain::LookUpAtRate);
}


/* ------- Move Function -------- */
void AMain::MoveForward(float value)
{
	if (CanMove(value)) {
		// Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);
	}
}
void AMain::MoveRight(float value)
{
	if (CanMove(value)) {
		// Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, value);
	}
}

bool AMain::CanMove(float Value)
{
	if (MainPlayerController)
	{
		return (Value != 0.0f) &&
			(!bAttacking) &&
			(MovementStatus != EMovementStatus::EMS_Dead) &&
			(!(MainPlayerController->bPauseMenuVisible));
	}
	return false;

	
}

void AMain::TurnAtRate(float rate)
{
	AddControllerYawInput(rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}
void AMain::LookUpAtRate(float rate)
{
	AddControllerPitchInput(rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMain::Turn(float yaw)
{
	if (CanMove(yaw))
	{
		AddControllerYawInput(yaw);
	}
}
void AMain::LookUp(float pitch)
{
	if (CanMove(pitch))
	{
		AddControllerPitchInput(pitch);
	}
}

void AMain::DecrementHealth(float Amount)
{
	if (Health - Amount <= 0.0f)
	{
		Health -= Amount;
		Die();
	}
	else
	{
		Health -= Amount;
	}
}

float AMain::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (Health - DamageAmount <= 0.0f)
	{
		Health -= DamageAmount;
		Die();
		if (DamageCauser)
		{
			AEnemy* Enemy = Cast<AEnemy>(DamageCauser);
			if (Enemy)
			{
				Enemy->bHasValidTarget = false;
			}
		}
	}
	else
	{
		Health -= DamageAmount;
	}

	return DamageAmount;
}

void AMain::Die()
{
	if ((MovementStatus == EMovementStatus::EMS_Dead))
		return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.0f);
		AnimInstance->Montage_JumpToSection(FName("Dead"), CombatMontage);
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetMovementStatus(EMovementStatus::EMS_Dead);
}

void AMain::DeathEnd()
{
	//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//if (AnimInstance)
	//{
		GetMesh()->bPauseAnims = true;
		GetMesh()->bNoSkeletonUpdate = true;
		//AnimInstance->Montage_Stop(0.5f, CombatMontage);
	//}
}

void AMain::Jump()
{
	if (MainPlayerController)
	{
		if (MainPlayerController->bPauseMenuVisible)
			return;
	}

	if ((MovementStatus != EMovementStatus::EMS_Dead))
	{
		ACharacter::Jump();
	}
	else
	{

	}
}

void AMain::IncrementCoins(int32 Amount)
{
	Coins += Amount;
}

void AMain::IncrementHealth(int32 Amount)
{
	if (Health + Amount >= MaxHealth)
	{
		Health = MaxHealth;
	}
	else
		Health += Amount;
}

// Pressed to enable Sprinting 
void AMain::ShiftKeyDown()
{
	bShiftKeyDown = true;
	UE_LOG(LogTemp, Warning, TEXT("Sprint True"));
}

// Released to stop
void AMain::ShiftKeyUp()
{
	bShiftKeyDown = false;
	UE_LOG(LogTemp, Warning, TEXT("Sprint False"));
}

void AMain::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;
	switch (MovementStatus)
	{
	case EMovementStatus::EMS_Sprinting : 
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
		break;

	case EMovementStatus::EMS_Normal :
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
		break;
	}
}

void AMain::MovingKeyDown()
{
	bMovingKeyDown = true;
}

void AMain::MovingKeyUp()
{
	bMovingKeyDown = false;
}

FRotator AMain::GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.0f, LookAtRotation.Yaw, 0.0f);
	return LookAtRotationYaw;
}

void AMain::SetInterpToEnemy(bool Interp)
{
	bInterpToEnemy = Interp;
}

void AMain::ShowPickupLoations()
{
	for (int32 i = 0; i < PickupLocations.Num(); i++)
	{
		UKismetSystemLibrary::DrawDebugSphere(this, PickupLocations[i], 25.0f, 8, FLinearColor::Green, 10.0f, 0.5f);
	}


	for (FVector Location : PickupLocations)
	{
		UKismetSystemLibrary::DrawDebugSphere(this, Location, 50.0f, 8, FLinearColor::Blue, 10.0f, 0.5f);
	}

}

void AMain::LMBDown()
{
	bLMBDown = true;

	if ((MovementStatus == EMovementStatus::EMS_Dead)) return;

	if (MainPlayerController)
	{
		if (MainPlayerController->bPauseMenuVisible)
			return;
	}

	if (ActiveOverlappingItem)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem);
		if (Weapon)
		{
			Weapon->Equip(this);
		}
	}
	else if (EquippedWeapon)
	{
		Attack();
	}
}
void AMain::LMBUp()
{
	bLMBDown = false;
}

void AMain::ESCDown()
{
	bESCDown = true;

	if (MainPlayerController)
	{
		MainPlayerController->TogglePauseMenu();
	}
}
void AMain::ESCUp()
{
	bESCDown = true;
}

void AMain::Attack()
{
	if (!bAttacking && (MovementStatus != EMovementStatus::EMS_Dead))
	{
		bAttacking = true;
		SetInterpToEnemy(true);

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && CombatMontage)
		{
			int32 Section = FMath::RandRange(0, 1);
			switch (Section)
			{
			case 0:
				AnimInstance->Montage_Play(CombatMontage, 2.2f);
				AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
				break;
			case 1:
				AnimInstance->Montage_Play(CombatMontage, 2.0f);
				AnimInstance->Montage_JumpToSection(FName("Attack_2"), CombatMontage);
				break;
			default:

				break;
			}
			
		}

	}

	
}

void AMain::AttackEnd()
{
	bAttacking = false;
	SetInterpToEnemy(false);
	if (bLMBDown)
	{
		Attack();
	}
}

void AMain::PlaySwingSound()
{
	if (EquippedWeapon->SwingSound)
	{
		UGameplayStatics::PlaySound2D(this, EquippedWeapon->SwingSound);
	}
}

void AMain::UpdateCombatTarget()
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateCombatTarget Start!!!"));
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, EnemyFilter); // 두 번째 매개변수는 겹친 것들 중에서 어떤 것만 고를지에 대한 필터링
	
	if (OverlappingActors.Num() == 0)
	{
		if (MainPlayerController)
		{
			MainPlayerController->RemoveEnemyHealthBar();
		}
		SetCombatTarget(nullptr);
		return; // 빼먹으면 런타임 오류난다! 크러쉬 발생
	}

	AEnemy* ClosestEnemy = Cast<AEnemy>(OverlappingActors[0]);
	if (ClosestEnemy)
	{
		FVector Location = GetActorLocation();
		float MinDistance = (ClosestEnemy->GetActorLocation() - Location).Size();

		for (auto Actor : OverlappingActors)
		{
			AEnemy* Enemy = Cast<AEnemy>(Actor);
			if (Enemy)
			{
				float DistanceToActor = (Enemy->GetActorLocation() - Location).Size();
				if (MinDistance > DistanceToActor)
				{
					MinDistance = DistanceToActor;
					ClosestEnemy = Enemy;
				}
			}
			
		}
		if (MainPlayerController)
		{
			MainPlayerController->DisplayEnemyHealthBar();
		}
		SetCombatTarget(ClosestEnemy);
		bHasCombatTarget = true;
	}
	else
	{
		SetCombatTarget(nullptr);
		bHasCombatTarget = false;
	}


}

void AMain::SwitchLevel(FName LevelName)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FString CurrentLevel = World->GetMapName();
		CurrentLevel.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
		
		FName CurrentLevelName(*CurrentLevel);

		if (CurrentLevelName != LevelName)
		{
			SaveGame();
			UGameplayStatics::OpenLevel(World, LevelName);
		}
	}
}


void AMain::SaveGame()
{
	//ChangedLevel += 100;
	UFirstSaveGame* SaveGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));

	SaveGameInstance->CharacterStats.Health = Health;
	SaveGameInstance->CharacterStats.MaxHealth = MaxHealth;
	SaveGameInstance->CharacterStats.Stamina = Stamina;
	SaveGameInstance->CharacterStats.MaxStamina = MaxStamina;
	SaveGameInstance->CharacterStats.Coins = Coins;

	FString MapName = GetWorld()->GetMapName();
	MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	UE_LOG(LogTemp, Warning, TEXT("MapName : %s"), *MapName);

	SaveGameInstance->CharacterStats.LevelName = MapName;

	if (EquippedWeapon)
	{
		SaveGameInstance->CharacterStats.WeaponName = EquippedWeapon->Name;
	}

	SaveGameInstance->CharacterStats.Location = GetActorLocation();
	SaveGameInstance->CharacterStats.Rotation = GetActorRotation();
	
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->PlayerName, SaveGameInstance->UserIndex);

	
}


void AMain::LoadGame(bool bSetPosition)
{
	UFirstSaveGame* LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));

	LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));

	Health = LoadGameInstance->CharacterStats.Health;
	MaxHealth = LoadGameInstance->CharacterStats.MaxHealth;
	Stamina = LoadGameInstance->CharacterStats.Stamina;
	MaxStamina = LoadGameInstance->CharacterStats.MaxStamina;
	Coins = LoadGameInstance->CharacterStats.Coins;

	if (WeaponStorage)
	{
		AItemStorage* Weapons = GetWorld()->SpawnActor<AItemStorage>(WeaponStorage);
		if (Weapons)
		{
			FString WeaponName = LoadGameInstance->CharacterStats.WeaponName;

			if (WeaponName != TEXT(""))
			{
				if (Weapons->WeaponMap.Contains(WeaponName))
				{
					AWeapon* WeaponToEquip = GetWorld()->SpawnActor<AWeapon>(Weapons->WeaponMap[WeaponName]);
					WeaponToEquip->Equip(this);
				}
			}
		}
	}
	

	if (bSetPosition)
	{
		SetActorLocation(LoadGameInstance->CharacterStats.Location);
		SetActorRotation(LoadGameInstance->CharacterStats.Rotation);
	}


	if (GetCapsuleComponent()->GetCollisionEnabled() == ECollisionEnabled::NoCollision)
	{
		SetMovementStatus(EMovementStatus::EMS_Normal);
		GetMesh()->bPauseAnims = false;
		GetMesh()->bNoSkeletonUpdate = false;

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Stop(0.0f, CombatMontage);
		}

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
		
	if (LoadGameInstance->CharacterStats.LevelName != TEXT(""))
	{
		FName LevelName(*LoadGameInstance->CharacterStats.LevelName);

		SwitchLevel(LevelName);
	}

	UE_LOG(LogTemp, Warning, TEXT("Loaded!"));
}

void AMain::LoadGameNoSwitch()
{
	UFirstSaveGame* LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));
	//UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	//LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));
	//if (MyGameInstance->GetIsFirst() == 0)
	//	return;
		
	//MyGameInstance->SetIsFirst(1);

	Health = LoadGameInstance->CharacterStats.Health;
	MaxHealth = LoadGameInstance->CharacterStats.MaxHealth;
	Stamina = LoadGameInstance->CharacterStats.Stamina;
	MaxStamina = LoadGameInstance->CharacterStats.MaxStamina;
	Coins = LoadGameInstance->CharacterStats.Coins;

	
	if (WeaponStorage)
	{
		AItemStorage* Weapons = GetWorld()->SpawnActor<AItemStorage>(WeaponStorage);
		if (Weapons)
		{
			FString WeaponName = LoadGameInstance->CharacterStats.WeaponName;

			if (WeaponName != TEXT(""))
			{
				if (Weapons->WeaponMap.Contains(WeaponName))
				{
					AWeapon* WeaponToEquip = GetWorld()->SpawnActor<AWeapon>(Weapons->WeaponMap[WeaponName]);
					WeaponToEquip->Equip(this);
				}
			}
		}
	}
	


	if (GetCapsuleComponent()->GetCollisionEnabled() == ECollisionEnabled::NoCollision)
	{
		SetMovementStatus(EMovementStatus::EMS_Normal);
		GetMesh()->bPauseAnims = false;
		GetMesh()->bNoSkeletonUpdate = false;

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Stop(0.0f, CombatMontage);
		}

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}


	UE_LOG(LogTemp, Warning, TEXT("NoSwitchLoaded!"));

}