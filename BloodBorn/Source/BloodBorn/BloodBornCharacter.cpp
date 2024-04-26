/// Copyright Epic Games, Inc. All Rights Reserved.

#include "BloodBornCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Animation/AnimMontage.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Items/Weapons/Gun.h"
#include "Items/BloodVial.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AttributeComponent.h"
#include "HUD/BBPlayerHUD.h"
#include "HUD/PlayerOverlay.h"
#include "Interfaces/HitInterface.h"
#include "BBLegacyCameraShake.h"
/*#include "../../../../../../../Source/Runtime/Engine/Classes/Engine/World.h"*/
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "BloodBornGameMode.h"
#include "LevelActor/BloodBornGameInstance.h"
#include "GameFramework/ForceFeedbackEffect.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ABloodBornCharacter

ABloodBornCharacter::ABloodBornCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 8.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// sawMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Saw Mesh Comp"));
	// sawMeshComp->SetupAttachment(GetMesh());
	// ConstructorHelpers::FObjectFinder<USkeletalMesh> TempSawMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/76MJ/Character/Weapon/saw-cleaver-bloodborne/source/SK_WeaponSaw.SK_WeaponSaw'"));
	// if (TempSawMesh.Succeeded())
	// {
	//	sawMeshComp->SetSkeletalMesh((TempSawMesh.Object))-;
	// }

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	LastHitTime = -10000000.f;
	RallyWindowDuration = 5.0f;
	// LastDamageReceived = 0.0f;

	regainHealEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RegainHeal Effect"));
	regainHealEffect->SetupAttachment(GetMesh(), FName("NeckSocket"));
	regainHealEffect->bAutoActivate = false;

	healEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Heal Effect"));
	healEffect->SetupAttachment(GetMesh(), FName("healSocket"));
	healEffect->bAutoActivate = false;

	makeBulletEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MakeBullet Effect"));
	makeBulletEffect->SetupAttachment(GetMesh(), FName("MakeBulletSocket"));
	makeBulletEffect->bAutoActivate = false;

	finishBulletEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FinishBullet Effect"));
	finishBulletEffect->SetupAttachment(GetMesh(), FName("NeckSocket"));
	finishBulletEffect->bAutoActivate = false;

}

void ABloodBornCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeaponSaw && EquippedWeaponSaw->GetSawBox())
	{
		EquippedWeaponSaw->GetSawBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeaponSaw->IgnoreActors.Empty();
	}
}

void ABloodBornCharacter::HoldAttackStart(AActor* DamagedActor, float DamageAmount, EAttackType AttackType)
{
	attackType = 0;
	EquippedWeaponSaw->ApplyDamage(LockOnEnemyREF, DamageAmount, AttackType);
	//FollowCamera->FieldOfView = 70;

	UWorld* World = GetWorld();
// 	const float CurrentFOV = FollowCamera->FieldOfView;
// 	UE_LOG(LogTemp, Log, TEXT("Current FOV: %f"), CurrentFOV);
// 	const float InterpSpeed = 0.07f;
// 		FollowCamera->FieldOfView = FMath::FInterpTo(CurrentFOV,
// 			70.f,
// 			World->GetTimeSeconds(),
// 			InterpSpeed);
	//UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(UBBLegacyCameraShake::StaticClass(), 0.15f);
	//UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(UBBLegacyCameraShake::StaticClass(), 1.f);
}

void ABloodBornCharacter::HoldAttackEnd(AActor* DamagedActor, float DamageAmount, EAttackType AttackType)
{
	attackType = 2;
	EquippedWeaponSaw->ApplyDamage(LockOnEnemyREF, DamageAmount, AttackType);
	// FollowCamera->FieldOfView = 90;

// 	UWorld* World = GetWorld();
// 	const float CurrentFOV = FollowCamera->FieldOfView;
// 	UE_LOG(LogTemp, Log, TEXT("Current FOV: %f"), CurrentFOV);
// 	const float InterpSpeed = 0.07f;
// 	FollowCamera->FieldOfView = FMath::FInterpTo(CurrentFOV, 90.0f, World->GetTimeSeconds(), InterpSpeed);
	//UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(UBBLegacyCameraShake::StaticClass(), 0.07f);
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(UBBLegacyCameraShake::StaticClass(), 0.25f);
}

void ABloodBornCharacter::WeaponVisible()
{
	//if (EquippedWeaponSaw != nullptr && EquippedWeaponSaw->ItemMesh != nullptr)
	//{
// 		EquippedWeaponSaw->ItemMesh->SetVisibility(true);
// 		ActionState = EActionState::EAS_Unoccupied;

	//}
	EquippedWeaponSaw->ItemMesh->SetVisibility(true);
	ActionState = EActionState::EAS_Unoccupied;
}

void ABloodBornCharacter::WeaponInvisible()
{
	EquippedWeaponSaw->ItemMesh->SetVisibility(false);
}

void ABloodBornCharacter::GunVisible()
{
	EquippedGun->ItemMesh->SetVisibility(true);
	ActionState = EActionState::EAS_Unoccupied;
}

void ABloodBornCharacter::GunInvisible()
{
	EquippedGun->ItemMesh->SetVisibility(false);
}

void ABloodBornCharacter::BeginPlay()
{
	Super::BeginPlay();
	pc = Cast<APlayerController>(Controller);

	if (pc != nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		//if (PlayerController)  // 125번째에 가있어야 하는 게 아닌지 ..
		//{
		ABBPlayerHUD* BBPlayerHUD = Cast<ABBPlayerHUD>(pc->GetHUD());
		if (BBPlayerHUD)
		{	
			UE_LOG(LogTemp, Warning, TEXT("HUD valid"));

			/*UPlayerOverlay* */PlayerOverlay = BBPlayerHUD->GetPlayerOverlay();
			if (PlayerOverlay && Attributes)
			{
				PlayerOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				PlayerOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
				PlayerOverlay->SetStaminaSliderBarPercent(Attributes->GetStaminaSlider());
				PlayerOverlay->SetDecreaseStaminaBarPercent(Attributes->GetDecreaseStaminaPercent());
				PlayerOverlay->SetHealthSliderBarPercent(Attributes->GetHealthSlider());
				PlayerOverlay->SetVial(Attributes->GetBloodVial());
				PlayerOverlay->SetBullet(Attributes->GetBullet());
				UE_LOG(LogTemp, Warning, TEXT("hpbar valid"));
			}
			DieOverlay = BBPlayerHUD->GetDieOverlay();
		}
	}
	UBloodBornGameInstance* gi = Cast<UBloodBornGameInstance>(GetGameInstance());
	if (gi->bHadSaw == true)
	{
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AWeapon* OverlappingWeapon = Cast<AWeapon>(GetWorld()->SpawnActor<AWeapon>(WeaponSaw, FVector::ZeroVector, FRotator::ZeroRotator, params));
		if (OverlappingWeapon)
		{
			OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
			OverlappingWeapon->SetOwner(this);
			//GetOwner()
			OverlappingWeapon->SetInstigator(this);
			//CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			OverlappingItem = nullptr;
			OverlappingWeapon->sawEffect->DestroyComponent(true);
			EquippedWeaponSaw = OverlappingWeapon;
		}
	}
	if (gi->bHadGun == true)
	{	
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AGun* OverlappingGun = Cast<AGun>(GetWorld()->SpawnActor<AGun>(WeaponGun, FVector::ZeroVector, FRotator::ZeroRotator, params));
		if (OverlappingGun)
		{
			OverlappingGun->Equip(GetMesh(), FName("LeftHandSocket"), this, this);
			OverlappingGun->SetOwner(this);
			//GetOwner()
			OverlappingGun->SetInstigator(this);
			//CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			OverlappingItem = nullptr;
			OverlappingGun->gunEffect->DestroyComponent(true);
			EquippedGun = OverlappingGun;
		}
	}
}

void ABloodBornCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FlushPersistentDebugLines(GetWorld());

	if (Attributes && PlayerOverlay)
	{
// 		if (Attributes->Stamina < Attributes->MaxStamina)
// 		{
		Attributes->RegenStamina(DeltaTime);
		Attributes->DeclineStamina(DeltaTime);
		PlayerOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
		PlayerOverlay->SetStaminaSliderBarPercent(Attributes->GetStaminaSlider());
		PlayerOverlay->SetDecreaseStaminaBarPercent(Attributes->GetDecreaseStaminaPercent());
//		}
	}

	if(Attributes->RegainableHealth > 0)
	{
		if(GetWorld()->GetTimeSeconds() - LastHitTime > 10.0f){
			Attributes->ResetRegainableHealth();
			if (PlayerOverlay && Attributes)
			{
				PlayerOverlay->SetPredictedHPBarPercent(Attributes->GetRegainHealthPercent());
			}
		}
	}
	/*IHitInterface* HitInterface = Cast<IHitInterface>(Hit.GetActor());
			//HitResult 인터페이스를 가지고 있으면
			if (HitInterface) {
				//플레이어와 적과의 거리를 계산
				FVector hitLocation =  Hit.GetActor()->GetActorLocation();
				float betweenDistance = (GetActorLocation() - hitLocation).Length();
				//이미 락온대상이 된 적이 있다면, 플레이어와 거리가 더 짧은 대상을 락온대상으로 교체
				if (betweenDistance < lockOnEnemyDistance) {
					lockOnEnemyDistance = betweenDistance;
					closestEnemy = Hit.GetActor();
				}
			}*/

	//록온중인경우
	if (bIsLockOn) {
		//록온된 상대의 인터페이스에 접근. 인터페이스가 있는 액터만 록온되므로 방어코드는 짜지 않았다.
		IHitInterface* HitInterface = Cast<IHitInterface>(LockOnEnemyREF);
		//록온된 상대의 체력이 0이하이면 = 죽었으면
		if (HitInterface->GetHealth() <= 0) {
			//록온함수를 재실행 = 록온 해제
			LockOn();
		}
		//살아있으면
		else {
			//적과의 거리를 계산한다. 내장뽑기용.
			FVector temp = LockOnEnemyREF->GetActorLocation() - GetActorLocation();
			lockOnEnemyDistance = temp.Length();
			//플레이어가 적을 바라보는 방향을 계산한다.
			FRotator lookAt = temp.Rotation();
			//컨트롤러의 조작 방향을 변경한다.(피치, 요, 롤). 락온하면 피치 조작계가 반대로 뒤집어져셔 -를 적용함
			Controller->SetControlRotation(FRotator(-mouseInputUpDown, lookAt.Yaw, lookAt.Roll));
		}
	}

	if(staCooldown > 0.0f){
		staCooldown -= DeltaTime;
	}

	if (bIsZoomIn) {
		CameraZoomInStart(DeltaTime);
	}
	if (bIsZoomOut) {
		CameraZoomOutStart(DeltaTime);
	}
}



//////////////////////////////////////////////////////////////////////////
// Input

void ABloodBornCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABloodBornCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABloodBornCharacter::Look);

		// Attacking
		EnhancedInputComponent->BindAction(NormalAttackAction, ETriggerEvent::Started, this, &ABloodBornCharacter::NormalAttack);

		EnhancedInputComponent->BindAction(StrongAttackAction, ETriggerEvent::Started, this, &ABloodBornCharacter::StrongAttack);

		// Equip
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ABloodBornCharacter::FKeyPressed);

		//Dodge
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &ABloodBornCharacter::Dodge);

		EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Started, this, &ABloodBornCharacter::LockOn);
		
		//사격공격
		EnhancedInputComponent->BindAction(GunFireAction, ETriggerEvent::Started, this, &ABloodBornCharacter::GunFire);

		// Heal - 수혈액 사용
		EnhancedInputComponent->BindAction(UseBloodVialAction, ETriggerEvent::Started, this, &ABloodBornCharacter::UseBloodVial);

		// 은탄 생성
		EnhancedInputComponent->BindAction(MakeBulletAction, ETriggerEvent::Started, this, &ABloodBornCharacter::MakeBullets);

		// 중단 메뉴 표시
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ABloodBornCharacter::ShowMenu);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ABloodBornCharacter::Move(const FInputActionValue& Value)
{
	Selection = 0;
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (ActionState == EActionState::EAS_StrongAttacking || ActionState == EActionState::EAS_HitReaction || ActionState == EActionState::EAS_HoldAttacking || ActionState == EActionState::EAS_GunFire) return;	 // 공격할 때 못 움직이게 강제, 근데 이렇게 하면 강공격 때도 또 받아야하는 거 아닌가 ? OR로 처리?

	GetCharacterMovement()->GetLastUpdateVelocity();
	GetControlRotation();


	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		// UE_LOG(LogTemp, Warning, TEXT("NOTLOCKONSTATE"));

		//if (CharacterState == ECharacterState::ECS_LockOn)
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("LOCKONSTATE"));  // 락온 풀어도 눌러도 계속 락온 state라는데 .. 뭘까 
		//}
	}
}

void ABloodBornCharacter::Look(const FInputActionValue& Value)
{
	if (bIsShowingMenu) {
		return;
	}
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);

		mouseInputUpDown += LookAxisVector.Y;
	}
}

void ABloodBornCharacter::FKeyPressed()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		OverlappingWeapon->SetOwner(this);
		//GetOwner()
		OverlappingWeapon->SetInstigator(this);
		//CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		OverlappingItem = nullptr;
 	 	OverlappingWeapon->sawEffect->DestroyComponent(true);
		UGameplayStatics::PlaySound2D(GetWorld(), equipSound);
		EquippedWeaponSaw = OverlappingWeapon;
	}

	AGun* OverlappingGun = Cast<AGun>(OverlappingItem);
	if (OverlappingGun)
	{
		OverlappingGun->Equip(GetMesh(), FName("LeftHandSocket"), this, this);
		OverlappingGun->SetOwner(this);
		//GetOwner()
		OverlappingGun->SetInstigator(this);
		//CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		OverlappingItem = nullptr;
		OverlappingGun->gunEffect->DestroyComponent(true);
		UGameplayStatics::PlaySound2D(GetWorld(), equipSound);
		EquippedGun = OverlappingGun;
	}

	ABloodVial* OverlappingBloodVial = Cast<ABloodVial>(OverlappingItem);
	if (OverlappingBloodVial)
	{
		OverlappingBloodVial->Equip(this/*, this*/);
// 		OverlappingBloodVial->SetOwner(this);
// 		OverlappingBloodVial->SetInstigator(this);
		OverlappingItem = nullptr;
		EquippedBV = OverlappingBloodVial;
	}

	/***************************************************************/
	/*OverlappingWeapon->ApplyDamage(락온된놈, 1000, ENUM::holdattack)*/

	//else
	//{
	//	if (ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped)
	//	{
	//	}
	//}
}

void ABloodBornCharacter::Dodge()
{
	if (CharacterState == ECharacterState::ECS_LockOn)
	{
		UE_LOG(LogTemp, Warning, TEXT("current state : LockOn"));
	}
	if (ActionState == EActionState::EAS_Unoccupied)
	{
		UE_LOG(LogTemp, Warning, TEXT("current state : Unoccupied"));
	}
	if (ActionState == EActionState::EAS_HitReaction)
	{
		UE_LOG(LogTemp, Warning, TEXT("current state : Hit!!!!!!!!!!!!!!!!!!!!"));
	}

	//Selection = 0;
	if (!HasEnoughStamina(Attributes->GetDodgeCost())) {
		UE_LOG(LogTemp, Warning, TEXT("NOOOOOOOOOOOOOOOHasEnoughStamina"));
	return;
	}
	else
	{
		if (ActionState == EActionState::EAS_Heal || ActionState == EActionState::EAS_MakeBullet)
		{
// 			if (bIsLockOn/* == false*/)
// 			{
// 				CharacterState = ECharacterState::ECS_LockOn;
// 				if ( CharacterState == ECharacterState::ECS_LockOn )
// 				{
// 					UE_LOG(LogTemp, Warning, TEXT("LockOn STATE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
// 				}
// 			}
// 			else
// 			{
// 				CharacterState = ECharacterState::ECS_Unequipped;
// 				
// 			}
			EquippedWeaponSaw->ItemMesh->SetVisibility(true);
			EquippedGun->ItemMesh->SetVisibility(true);
			GetCharacterMovement()->MaxWalkSpeed = 1000.f;
			GetCharacterMovement()->MaxAcceleration = 2048.f;
		}

		
		if (/*(CharacterState != ECharacterState::ECS_LockOn || CharacterState == ECharacterState::ECS_EquippedOneHandedWeapon)*/CharacterState == ECharacterState::ECS_Unequipped && ActionState != EActionState::EAS_HitReaction && (ActionState == EActionState::EAS_Unoccupied || ActionState == EActionState::EAS_Heal || ActionState == EActionState::EAS_MakeBullet))
		{
// 			
// 			if (bIsLockOn == false)
// 			{
// 				if (ActionState == EActionState::EAS_Heal || ActionState == EActionState::EAS_MakeBullet)
// 				{
// 					EquippedWeaponSaw->ItemMesh->SetVisibility(true);
// 					EquippedGun->ItemMesh->SetVisibility(true);
// 
// 					GetCharacterMovement()->MaxWalkSpeed = 1000.f;
// 					GetCharacterMovement()->MaxAcceleration = 2048.f;
// 					// GetCharacterMovement()->MaxWalkSpeed = 1000.f;
// 				}
				GetCharacterMovement()->MaxWalkSpeed = 1000.f;
				GetCharacterMovement()->MaxAcceleration = 2048.f;
				PlayDodgeMontage();
				ActionState = EActionState::EAS_Dodge;
				if (Attributes && PlayerOverlay)
				{
					Attributes->UseStamina(Attributes->GetDodgeCost());
					PlayerOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
					PlayerOverlay->SetStaminaSliderBarPercent(Attributes->GetStaminaSlider());
					PlayerOverlay->SetDecreaseStaminaBarPercent(Attributes->GetDecreaseStaminaPercent());
					PlayerOverlay->SetHealthSliderBarPercent(Attributes->GetHealthSlider());

				}
//			}
		}

		//ActionState = EActionState::EAS_Step;
		if (CharacterState == ECharacterState::ECS_LockOn  && ActionState != EActionState::EAS_HitReaction && (ActionState == EActionState::EAS_Unoccupied || ActionState == EActionState::EAS_Heal || ActionState == EActionState::EAS_MakeBullet/* || ActionState == EActionState::EAS_Step*/))
		{
			UE_LOG(LogTemp, Warning, TEXT("11111111111111111111111111111"));
			//bIsLockOn = true;

			bIsDodge = true;
			FVector currentVelocity = GetCharacterMovement()->Velocity;
			FRotator currentRot = GetCharacterMovement()->GetLastUpdateRotation()/*GetActorRotation()*/;

			FVector newVector = (currentRot.UnrotateVector(currentVelocity)).GetSafeNormal();
			moveX = newVector.X;
			moveY = newVector.Y;

			GetCharacterMovement()->MaxWalkSpeed = 1000.f;
			GetCharacterMovement()->MaxAcceleration = 4096.f;
			//UE_LOG(LogTemp, Warning, TEXT("movex and y :  %f, %f"), moveX, moveY);
			// PlayStepMontage();
			if (Attributes)
			{
				Attributes->UseStamina(Attributes->GetDodgeCost());
				PlayerOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
				PlayerOverlay->SetStaminaSliderBarPercent(Attributes->GetStaminaSlider());

			}
			FTimerHandle endtimer;
			GetWorldTimerManager().SetTimer(endtimer, FTimerDelegate::CreateLambda([&](){
				bIsDodge=false;
				GetCharacterMovement()->MaxWalkSpeed = 600.f;
				GetCharacterMovement()->MaxAcceleration = 2048.f;
			}), 0.4f, false);
			
			if (ActionState == EActionState::EAS_Step)
			{
				UE_LOG(LogTemp, Warning, TEXT("EAS_Step"));

			}
		}
	}
}

void ABloodBornCharacter::PlayDodgeMontage()
{
	//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	PlayMontageSection(DodgeMontage, FName("Default"));
}

void ABloodBornCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& sectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(sectionName, Montage);
	}
}

AActor* ABloodBornCharacter::EngageLockOn()
{
	//락온 범위 내의 액터 데이터를 받을 배열 생성
	TArray<FHitResult> hitInfos;
	//락온 범위 지정. 원형 750.0f
	FCollisionShape MySphere = FCollisionShape::MakeSphere(750.0f);
	//락온 범위의 락온 대상을 찾는다.
	bool bResult = GetWorld()->SweepMultiByObjectType(hitInfos, GetActorLocation(), GetActorLocation(), FQuat::Identity, ECC_WorldDynamic, MySphere);
	//대상을 찾은 경우
	if (bResult) {
		AActor* closestEnemy = nullptr;
		//대상들 배열 전체를 탐색
		for (auto& Hit : hitInfos) {
			//HitResult 인터페이스를 가지고 있다 = 적이다. 인터페이스를 갖고 있는지 확인.
			IHitInterface* HitInterface = Cast<IHitInterface>(Hit.GetActor());
			//HitResult 인터페이스를 가지고 있으면
			if (HitInterface) {
				//플레이어와 적과의 거리를 계산
				FVector hitLocation =  Hit.GetActor()->GetActorLocation();
				float betweenDistance = (GetActorLocation() - hitLocation).Length();
				//이미 락온대상이 된 적이 있다면, 플레이어와 거리가 더 짧은 대상을 락온대상으로 교체
				if (betweenDistance < lockOnEnemyDistance) {
					lockOnEnemyDistance = betweenDistance;
					closestEnemy = Hit.GetActor();
				}
			}
		}

		if (closestEnemy) {
			//락온 대상이 있으면 녹색 디버그 구체 생성
			//DrawDebugSphere(GetWorld(), GetActorLocation(), 750.0f, 100, FColor::Green, false, 3.0f);
			GetWorld()->GetFirstPlayerController()->SetIgnoreLookInput(true);
			GetCharacterMovement()->bOrientRotationToMovement = false;
			GetCharacterMovement()->bUseControllerDesiredRotation = true;
			bIsLockOn = true;

			CharacterState = ECharacterState::ECS_LockOn;

			// 락온 ui 작업
			// hit 인터페이스의 lockon 함수 호출 후 bool value가 true면 락온 실행(ui랑 체력바)
			// false면 락온 해제됨 여기는 true만 ~ 

			IHitInterface* HitInterface = Cast<IHitInterface>(closestEnemy);
			HitInterface->Lockon(true);

			return closestEnemy;
		}
// 		else {
// 			//스피어 트레이스 대상은 있었으나 락온 대상이 없으면 노란색 디버그 구체 생성
// 			//DrawDebugSphere(GetWorld(), GetActorLocation(), 750.0f, 100, FColor::Yellow, false, 3.0f);
// 		}
	}
// 	else {
// 		//스피어 트레이스 대상이 없으면 빨간 디버그 구체 생성
// 		//DrawDebugSphere(GetWorld(), GetActorLocation(), 750.0f, 100, FColor::Red, false, 3.0f);
// 	}
	return nullptr;
}

void ABloodBornCharacter::DisEngageLockOn()
{
	// 락온 ui 작업
	// false면 락온 해제됨
	// 여기서 hitinterface의 lockon 함수의 bool false인자를 전달헀음 밑에 
	IHitInterface* HitInterface = Cast<IHitInterface>(LockOnEnemyREF);
	HitInterface->Lockon(false);
	GetWorld()->GetFirstPlayerController()->SetIgnoreLookInput(false);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

void ABloodBornCharacter::NormalAttack()
{
	//if (bIsLockOn)
	//{

	//}
	//락온된 에너미가 있고,
	if (LockOnEnemyREF != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("11111"));
		//록온된 상대의 인터페이스에 접근. 인터페이스가 있는 액터만 록온되므로 방어코드는 짜지 않았다.
		IHitInterface* HitInterface = Cast<IHitInterface>(LockOnEnemyREF);

		//락온된 적이 스턴상태이고, 거리가 150.0f일 경우
		if (HitInterface->GetInStun() && lockOnEnemyDistance < 150.0f) {
			UE_LOG(LogTemp, Warning, TEXT("22222"));

			//무기장비 방어코드
			if (EquippedWeaponSaw != nullptr) {
				UE_LOG(LogTemp, Warning, TEXT("33333"));

				//여기서는 애니매이션만 실행시키고, 실제 대미지 처리는 애니메이션 블루프린트에서 하기
				//내장뽑기 공격타입
				attackType = 2;
				if (ActionState == EActionState::EAS_Unoccupied)
				{
					// EquippedWeaponSaw->SetActorHiddenInGame(true);  // 무기 없앴으니 이제 노티파이 부분에서 applydamage처리
					PlayHoldAttackMontage();
					// ApplyDamage(LockOnEnemyREF, 500, EAttackType::HoldAttack);
					ActionState = EActionState::EAS_HoldAttacking;
				}

				//내장뽑기가 발동된 경우 일반공격은 실행하지 않고 함수 종료
				// return;
			}
		}
	}

	//내장뽑기 조건이 달성되지 않으면 일반공격 명령이 실행
	if (!HasEnoughStamina(Attributes->GetNormalAttackCost())) return;
	else
	{
		attackType = 0;
		// if (CanAttack()) 그리고 캐릭터 헤더에  bool CanAttack(); 선언하고 cpp 에 정의해야함 선언 안했지만 선언 후 정의하면 쓸 코드 주석으로 작성
		// {PlayN~ EAS_Normal 고대로 }
		// bool ABloodBornCharacter::CanAttack(){return 	ActionState == EActionState::EAS_Unoccupied && CharacterState != EcharacterState::ECS_Unequipped;}
		if (ActionState == EActionState::EAS_Unoccupied)   // 맨손 공격 안하려면 주석으로 쓰기
		{
			GetCharacterMovement()->MaxWalkSpeed = 100.f;
			PlayNormalAttackMontage();
			ActionState = EActionState::EAS_NormalAttacking;
			//float Damage = EquippedWeapon->OnBoxOverlap()

			if (Attributes && PlayerOverlay)
			{
				Attributes->UseStamina(Attributes->GetNormalAttackCost());
				PlayerOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
				PlayerOverlay->SetStaminaSliderBarPercent(Attributes->GetStaminaSlider());
				PlayerOverlay->SetDecreaseStaminaBarPercent(Attributes->GetDecreaseStaminaPercent());
				PlayerOverlay->SetHealthSliderBarPercent(Attributes->GetHealthSlider());
			}
		}
		staCooldown = 2.0f;
	}
}



void ABloodBornCharacter::PlayNormalAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && NormalAttackMontage)  // if (AnimInstance && NormalAttackMontage)
	{
		//OriginMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
		//GetCharacterMovement()->MaxWalkSpeed = AttackMaxWalkSpeed;


		AnimInstance->Montage_Play(NormalAttackMontage);  // 나머지 파라미터들은 기본설정으로 일단 할거라 안 씀
		//const int32 Selection = FMath::RandRange(0, 2);

		FName SectionName = FName();  // FName 사용 쉽게
		switch (Selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		default:
			break;
		}
		Selection += 1;
		Selection %= 2;
		AnimInstance->Montage_JumpToSection(SectionName, NormalAttackMontage);
		MakeNoise(1.0f);
	}
}

void ABloodBornCharacter::StrongAttack()
{
	if (!HasEnoughStamina(Attributes->GetHeavyAttackCost())) return;
	else
	{
	
		attackType = 1;
		if (ActionState == EActionState::EAS_Unoccupied)
		{
			PlayStrongAttackMontage();
			ActionState = EActionState::EAS_StrongAttacking;
			if (Attributes && PlayerOverlay) 
			{
				Attributes->UseStamina(Attributes->GetHeavyAttackCost());
				PlayerOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
				PlayerOverlay->SetStaminaSliderBarPercent(Attributes->GetStaminaSlider());
				PlayerOverlay->SetDecreaseStaminaBarPercent(Attributes->GetDecreaseStaminaPercent());

			}
		}
		staCooldown = 2.0f;
	}
}

void ABloodBornCharacter::PlayStrongAttackMontage()
{
	PlayMontageSection(StrongAttackMontage, FName("HeavyAttack"));
	MakeNoise(1.0f);
}

void ABloodBornCharacter::PlayHoldAttackMontage()
{
	PlayMontageSection(HoldAttackMontage, FName("HoldAttack"));
}

void ABloodBornCharacter::AttackEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
 	//if (CharacterState != ECharacterState::ECS_LockOn)
 	//{
		ActionState = EActionState::EAS_Unoccupied;
 	//}
 	//else
 	//{
 	//	ActionState = EActionState::EAS_Step;
 	//}
}

void ABloodBornCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

float ABloodBornCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!bIsInvincible)
	{
		HandleDamage(DamageAmount);
		SetHUDHealth();
		if (PlayerOverlay && Attributes)
		{
			PlayerOverlay->SetPredictedHPBarPercent(Attributes->GetRegainHealthPercent());
		}
	}
	return DamageAmount;
}

// 지금은 여기다 두지만 리팩토링함수라 맨 밑으로 빼도 됨
void ABloodBornCharacter::SetHUDHealth()
{
	if (PlayerOverlay && Attributes)
	{
		PlayerOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
		PlayerOverlay->SetHealthSliderBarPercent(Attributes->GetHealthSlider());
	}
} 

void ABloodBornCharacter::HandleDamage(float DamageAmount)
{
	if (!bIsInvincible && Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount);
		if (Attributes->Health == 0.0f)
		{
			Die();
			ActionState = EActionState::EAS_Dead;
			Attributes->ResetRegainableHealth();
			return;
		}
		else if (DamageAmount < 150.f)
		{
			PlayHitReactMontage();
			VibController(0);

			int32 RandomNumber = FMath::RandRange(0, 99);

			if (RandomNumber < 30) {
				UGameplayStatics::PlaySound2D(GetWorld(), hitSound1); // 30%의 확률로 hitSound1 재생
			}
			else if (RandomNumber < 40) {
				UGameplayStatics::PlaySound2D(GetWorld(), hitSound2); // 10%의 확률로 hitSound2 재생
			}

		}
		else if (DamageAmount >= 150.f)
		{
			PlayHeavyHitReactMontage();
			VibController(2);

			int32 RandomNumber = FMath::RandRange(0, 99);

			if (RandomNumber < 40) {
				UGameplayStatics::PlaySound2D(GetWorld(), heavyHitSound);
			}
			UE_LOG(LogTemp, Warning, TEXT("HEAVY DAMAGE"));	
		}

		GetCharacterMovement()->MaxWalkSpeed = 500.0f;  // 이건 왜 있는거지 
		UE_LOG(LogTemp, Warning, TEXT("Player Hit! : %f"), DamageAmount);
		ActionState = EActionState::EAS_HitReaction;
		LastHitTime = GetWorld()->GetTimeSeconds();  // 마지막 공격을 받은 시간 업데이트
	}
}

void ABloodBornCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}


void ABloodBornCharacter::UseBloodVial()
{
	if (EquippedWeaponSaw && ActionState != EActionState::EAS_HitReaction && ActionState == EActionState::EAS_Unoccupied) 
	{
 		if (Attributes)
 		{
 			if (Attributes->BloodVial > 0)
 			{
 				ActionState = EActionState::EAS_Heal;
//  				if (ActionState == EActionState::EAS_Heal)
//  				{
				GetCharacterMovement()->MaxWalkSpeed = 100.f;
				GetCharacterMovement()->MaxAcceleration = 512.f;
				PlayBloodVialMontage();

// 				}
 			}
// 			else
// 			{
// 				return;
// 			}
// 		}
//  		if (Attributes && PlayerOverlay)
// 	 	{
// 	 		if (Attributes->BloodVial > 0)
// 	 		{
// 	 			ActionState = EActionState::EAS_Heal;
// 	 			if(ActionState == EActionState::EAS_Heal)
// 	 			{
// 	 				GetCharacterMovement()->MaxWalkSpeed = 150.f;
// 	 				GetCharacterMovement()->MaxAcceleration = 512.f;
// 	 				Attributes->UseBloodVial(Attributes->GetBloodVial());
// 	 				PlayerOverlay->SetVial(Attributes->GetBloodVial());
// 	 				PlayerOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
// 	 				PlayerOverlay->SetHealthSliderBarPercent(Attributes->GetHealthSlider());
//	 				PlayBloodVialMontage();
//	 			}
	 	}
	}
	else
	{
		// 몽타주 재생 안함
		return;
	}
// 		}
// 	}
}

void ABloodBornCharacter::Heal()
{
	if (Attributes && PlayerOverlay)
	{
		if (Attributes->BloodVial > 0)
		{
			Attributes->UseBloodVial(Attributes->GetBloodVial());
			PlayerOverlay->SetVial(Attributes->GetBloodVial());
			PlayerOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
			PlayerOverlay->SetHealthSliderBarPercent(Attributes->GetHealthSlider());
	
/*		instanceEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(healEffect, GetRootComponent(), FName("healSocket"), EAttachLocation::KeepWorldPosition, true, true);*/
// 		FVector HealLocation;
// 		FRotator HealRotation;
// 		GetMesh()->GetSocketWorldLocationAndRotation(FName("healSocket"), HealLocation, HealRotation);

		// Niagara
/*		instanceEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),	healEffect,	HealLocation, HealRotation / *, FVector::OneVector* /);*/
			healEffect->Activate(true);

			FTimerHandle healtimer;
			GetWorldTimerManager().SetTimer(healtimer, FTimerDelegate::CreateLambda([&]() {
				regainHealEffect->Activate(true);
			}), 0.2f, false);
		}
	}
}


void ABloodBornCharacter::PlayBloodVialMontage()
{
	// PlayMontageSection(BloodVialMontage, FName("UseBloodVial"));
	PlayMontageSection(BloodVialMontage, FName("Heal"));
// 	if (CharacterState == ECharacterState::ECS_LockOn)
// 	{
// 		bIsLockOn = true;
// 	}
// 	else
// 	{
// 		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
// 		bIsLockOn = false;
// 	}
//	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
}




// void ABloodBornCharacter::AddBloodVials(ABloodVial* BloodVial)
// {
// 	if (Attributes)
// 	{
// 		Attributes->AddBloodVials(BloodVial->GetBloodVials());
// 		int32 BloodVialCount = Attributes->GetBloodVials();
// 		UE_LOG(LogTemp, Warning, TEXT("Blood Vial Count: %d"), BloodVialCount);
// 	}
// }

void ABloodBornCharacter::PlayHitReactMontage()
{
	PlayMontageSection(HitReactMontage, FName("HitReact"));
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(UBBLegacyCameraShake::StaticClass(), 0.05f);
	/*FollowCamera->FieldOfView = 70;*/
}

void ABloodBornCharacter::PlayHeavyHitReactMontage()
{
	PlayMontageSection(HeavyHitReactMontage, FName("HeavyHitReact"));
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(UBBLegacyCameraShake::StaticClass(), 0.1f);
}

void ABloodBornCharacter::Die()
{
	 PlayDeathMontage();
	 GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	 GetCharacterMovement()->DisableMovement();
	 //DisableInput(Ge); 카메라 움직임 멈추기?
	 CameraBoom->bUsePawnControlRotation = false;

	 //instanceEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), deadEffect, GetActorLocation(), FRotator::ZeroRotator, FVector(1.0f));
	 FVector CharacterLocation = GetActorLocation();

	 float CharacterHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - 55.0f;
	 FVector SpawnLocation = CharacterLocation - FVector(0.0f, 0.0f, CharacterHeight);

	 UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), deadEffect, SpawnLocation, FRotator::ZeroRotator, FVector(0.85f));
// 
// 	 instanceEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
// 		 GetWorld(),
// 		 deadEffect,
// 		 SpawnLocation,
// 		 FRotator::ZeroRotator,
// 		 FVector(0.75f)
// 	 );


	 FTimerHandle dietimer;
	 GetWorldTimerManager().SetTimer(dietimer, FTimerDelegate::CreateLambda([&]() {

		 if (pc != nullptr)
		 {

			 UGameplayStatics::PlaySound2D(GetWorld(), deathSound);
			 ABBPlayerHUD* BBPlayerHUD = Cast<ABBPlayerHUD>(pc->GetHUD());
			 if (BBPlayerHUD)
			 {
				 BBPlayerHUD->ShowDieOverlay();
			 }

		 }
	 }), 3.0f, false);
	

	 FTimerHandle OpenLevelTimer;
	 GetWorldTimerManager().SetTimer(OpenLevelTimer, FTimerDelegate::CreateLambda([&]() {
		 UGameplayStatics::OpenLevel(GetWorld(), FName("DemoMapDay_Copy"));
		 }
	 ), 8.f, false);
}

// 
// void ABloodBornCharacter::OpenLevelFunction()
// {
// 	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
// 	{
// 		UGameplayStatics::OpenLevel(GetWorld(), FName("PlayerLevel"));
// 	}
// }


void ABloodBornCharacter::PlayDeathMontage()
{
	PlayMontageSection(DeathMontage, FName("Death"));
	ActionState = EActionState::EAS_Dead;
}


bool ABloodBornCharacter::HasEnoughStamina(float staCost)
{
	// return Attributes && Attributes->GetStamina() > Attributes->GetDodgeCost();  // 퍼센트가 아니라 실제 스테미나 필요, 지금은 dodgecost로 일단 테스트 
	return Attributes && Attributes->GetStamina() > staCost;
}

void ABloodBornCharacter::PlayStepMontage()
{
	GetCharacterMovement()->MaxWalkSpeed = 1000.f;
	GetCharacterMovement()->MaxAcceleration = 4096.f;
	PlayMontageSection(StepMontage, FName("Step"));
}


void ABloodBornCharacter::LockOn()
{
	if (bIsLockOn)
	{
		CharacterState = ECharacterState::ECS_Unequipped;
		bIsLockOn = false;
		DisEngageLockOn();
		LockOnEnemyREF = nullptr;
		lockOnEnemyDistance = 800.0f;
	}
	else
	{
		LockOnEnemyREF = EngageLockOn();
	}
}

void ABloodBornCharacter::GunFire()
{
 	if (Attributes->Bullet > 0)
 	{
		if (ActionState == EActionState::EAS_Unoccupied && LockOnEnemyREF != nullptr) {
		
			ActionState = EActionState::EAS_GunFire;
			//록온된 적의 인터페이스에 접근
			IHitInterface* HitInterface = Cast<IHitInterface>(LockOnEnemyREF);
			HitInterface->GotParryAttackCPP(gunDamage);
			Attributes->UseBullet(Attributes->GetBullet());
			PlayFireMontage();
			VibController(2);
		}
 	}
 	else
 	{
 		return;
 	}
}

void ABloodBornCharacter::PlayFireMontage()
{

	PlayMontageSection(FireMontage, FName("GunFire"));
	if (PlayerOverlay)
	{
		PlayerOverlay->SetBullet(Attributes->GetBullet());
	}
	EquippedGun->fireEffect->Activate(true);
}

void ABloodBornCharacter::MakeBullets()
{
	if (EquippedGun && ActionState != EActionState::EAS_HitReaction && ActionState == EActionState::EAS_Unoccupied)
	{
		if (Attributes)
		{
			if (Attributes->Health > 100.f)
			{
				ActionState = EActionState::EAS_MakeBullet;

				//  				if (ActionState == EActionState::EAS_Heal)
				//  				{
				GetCharacterMovement()->MaxWalkSpeed = 100.f;
				GetCharacterMovement()->MaxAcceleration = 512.f;
				PlayMakeBulletMontage();
				// 				}
			}
		}
	}
	else
	{
		return;
	}
}

//메뉴표시 버튼을 누르면 메뉴가 표시된다. 메뉴가 표시되면 마우스 조작 이외의 입력이 막힌다.
void ABloodBornCharacter::ShowMenu()
{
	bIsShowingMenu = true;

	ABloodBornGameMode* gm = Cast<ABloodBornGameMode>(GetWorld()->GetAuthGameMode());
	gm->ShowPauseUI();
}

void ABloodBornCharacter::Decline()
{
	if (Attributes && PlayerOverlay)
	{
		if (Attributes->Health >= 100.f)
		{
			Attributes->MakeBullet(Attributes->GetBullet());
			PlayerOverlay->SetBullet(Attributes->GetBullet());
			PlayerOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
			PlayerOverlay->SetHealthSliderBarPercent(Attributes->GetHealthSlider());

// 			FVector BulletLocation;
// 			FRotator BulletRotation;
// 			GetMesh()->GetSocketWorldLocationAndRotation(FName("MakeBulletSocket"), BulletLocation, BulletRotation);
// 			// Niagara 시스템
// 			instanceEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), makeBulletEffect, BulletLocation, BulletRotation /*, FVector::OneVector*/);
			
			makeBulletEffect->Activate(true);

			FTimerHandle bullettimer;
			GetWorldTimerManager().SetTimer(bullettimer, FTimerDelegate::CreateLambda([&]() {
				/*instanceEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), finishBulletEffect, GetActorLocation(), FRotator::ZeroRotator, FVector(0.7f));*/
				finishBulletEffect->Activate(true);
			}), 0.05f, false);
		}
		else
		{
			return;
		}
	}
}


void ABloodBornCharacter::PlayMakeBulletMontage()
{
	PlayMontageSection(MakeBulletMontage, FName("MakeBullet"));
	//CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;

}



//void adf내장뽑기()
//{
//	if (bIsLockOn && 적이 스턴 상태 &&거리)
//	{
//	 내장뽑기
//	}
//}
void ABloodBornCharacter::OnSuccessfulAttack(float Damage, EAttackType AttackType)
{
	if (IsInRallyWindow())
	{
		float RegainedHealth = 0.0f;
		if (AttackType == EAttackType::Normal)
		{
			RegainedHealth = 2.0f;
		}
		else if (AttackType == EAttackType::Heavy)
		{
			RegainedHealth = 30.0f;
		}
		else {
			//내장뽑기 상황에는 리게인 전체 회복
			RegainedHealth = Attributes->RegainableHealth;

		}

		// Attributes->RegainHealth(RegainedHealth);
		// 회복 가능한 최대 체력을 계산
		//float MaxRegainableHealth = CalculateMaxRegainableHealth(Damage);

		//float RegainableHealth = Damage;

		//// 회복 가능한 체력이 있는 경우에만 회복 적용
		//if (RegainableHealth > 0.0f)
		//{
			// 회복 가능한 체력을 플레이어에게 적용
		Attributes->RegainHealth(RegainedHealth);
		// 회복 나이아가라
/*			instanceEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), regainHealEffect, GetActorLocation(), FRotator::ZeroRotator, FVector(0.5f));*/
		regainHealEffect->Activate(true);
		UGameplayStatics::PlaySound2D(GetWorld(), regainSound);
		SetHUDHealth();
		if (PlayerOverlay && Attributes)
		{
			PlayerOverlay->SetPredictedHPBarPercent(Attributes->GetRegainHealthPercent());
		}
		//}
	}
	else
	{
		Attributes->ResetRegainableHealth();
		//UE_LOG(LogTemp, Warning, TEXT("%f"), Attributes->RegainableHealth);
		if (PlayerOverlay && Attributes)
		{
			PlayerOverlay->SetPredictedHPBarPercent(Attributes->GetRegainHealthPercent());
		}

	}
}


//float ABloodBornCharacter::CalculateMaxRegainableHealth(float Damage)
//{
//	// 현재 체력과 받은 데미지를 더한 값이 회복 가능한 최대 체력
//	float MaxRegainableHealth = Attributes->GetHealth() + Damage;
//
//	return MaxRegainableHealth;
//}

bool ABloodBornCharacter::IsInRallyWindow() const
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	// return (CurrentTime - LastHitTime) <= RallyWindowDuration;
	float TimeSinceLastHit = CurrentTime - LastHitTime;

	if (TimeSinceLastHit <= RallyWindowDuration)
	{
		UE_LOG(LogTemp, Warning, TEXT("IsInRallyDuration"));
		return true;
	}
	else
	{	
		Attributes->ResetRegainableHealth();
		UE_LOG(LogTemp, Warning, TEXT("NOTInRallyDuration"));

		return false;
	}
}

void ABloodBornCharacter::CameraZoomInStart(float deltaTime)
{
	cameraZoomInTimer += deltaTime * zoomInInt;
	zoomInInt+=0.5f;
	FollowCamera->FieldOfView = FMath::Lerp(startFOV, endFOV, cameraZoomInTimer/ cameraZoomInAnimTime);
	UE_LOG(LogTemp, Warning, TEXT("ZoomIn : %f"), cameraZoomInTimer / cameraZoomInAnimTime);
	if (cameraZoomInTimer > cameraZoomInAnimTime) {
		bIsZoomIn = false;
		cameraZoomInTimer = 0.0f;
		zoomInInt = 1.0f;
		FTimerHandle zoomOutHandle;
		GetWorldTimerManager().SetTimer(zoomOutHandle, FTimerDelegate::CreateLambda([&](){
			bIsZoomOut = true;
		}), 0.9f, false);
	}
}

void ABloodBornCharacter::CameraZoomOutStart(float deltaTime)
{
	cameraZoomOutTimer += deltaTime;
	FollowCamera->FieldOfView = FMath::Lerp(endFOV, startFOV, cameraZoomOutTimer / cameraZoomOutAnimTime);
	UE_LOG(LogTemp, Warning, TEXT("ZoomOut : %f"), cameraZoomOutTimer / cameraZoomOutAnimTime);
	if (cameraZoomOutTimer > cameraZoomOutAnimTime) {
		cameraZoomOutTimer = 0.0f;
		bIsZoomOut = false;
	}
}

void ABloodBornCharacter::VibController(int32 vibType) {
	FForceFeedbackParameters params;
	if (pc != nullptr) {
		switch (vibType) {
		case 0:
			pc->ClientPlayForceFeedback(weekLeft, params);
			break;
		case 1:
			pc->ClientPlayForceFeedback(weekRight, params);
			break;
		case 2:
			pc->ClientPlayForceFeedback(strongLeft, params);
			break;
		case 3:
			pc->ClientPlayForceFeedback(strongRight, params);
			break;
		case 4:
			pc->ClientPlayForceFeedback(dual, params);
			break;
		}
	}
}