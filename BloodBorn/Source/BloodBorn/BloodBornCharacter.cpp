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

		//if (PlayerController)  // 125��°�� ���־�� �ϴ� �� �ƴ��� ..
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
			//HitResult �������̽��� ������ ������
			if (HitInterface) {
				//�÷��̾�� ������ �Ÿ��� ���
				FVector hitLocation =  Hit.GetActor()->GetActorLocation();
				float betweenDistance = (GetActorLocation() - hitLocation).Length();
				//�̹� ���´���� �� ���� �ִٸ�, �÷��̾�� �Ÿ��� �� ª�� ����� ���´������ ��ü
				if (betweenDistance < lockOnEnemyDistance) {
					lockOnEnemyDistance = betweenDistance;
					closestEnemy = Hit.GetActor();
				}
			}*/

	//�Ͽ����ΰ��
	if (bIsLockOn) {
		//�Ͽµ� ����� �������̽��� ����. �������̽��� �ִ� ���͸� �ϿµǹǷ� ����ڵ�� ¥�� �ʾҴ�.
		IHitInterface* HitInterface = Cast<IHitInterface>(LockOnEnemyREF);
		//�Ͽµ� ����� ü���� 0�����̸� = �׾�����
		if (HitInterface->GetHealth() <= 0) {
			//�Ͽ��Լ��� ����� = �Ͽ� ����
			LockOn();
		}
		//���������
		else {
			//������ �Ÿ��� ����Ѵ�. ����̱��.
			FVector temp = LockOnEnemyREF->GetActorLocation() - GetActorLocation();
			lockOnEnemyDistance = temp.Length();
			//�÷��̾ ���� �ٶ󺸴� ������ ����Ѵ�.
			FRotator lookAt = temp.Rotation();
			//��Ʈ�ѷ��� ���� ������ �����Ѵ�.(��ġ, ��, ��). �����ϸ� ��ġ ���۰谡 �ݴ�� ���������� -�� ������
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
		
		//��ݰ���
		EnhancedInputComponent->BindAction(GunFireAction, ETriggerEvent::Started, this, &ABloodBornCharacter::GunFire);

		// Heal - ������ ���
		EnhancedInputComponent->BindAction(UseBloodVialAction, ETriggerEvent::Started, this, &ABloodBornCharacter::UseBloodVial);

		// ��ź ����
		EnhancedInputComponent->BindAction(MakeBulletAction, ETriggerEvent::Started, this, &ABloodBornCharacter::MakeBullets);

		// �ߴ� �޴� ǥ��
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

	if (ActionState == EActionState::EAS_StrongAttacking || ActionState == EActionState::EAS_HitReaction || ActionState == EActionState::EAS_HoldAttacking || ActionState == EActionState::EAS_GunFire) return;	 // ������ �� �� �����̰� ����, �ٵ� �̷��� �ϸ� ������ ���� �� �޾ƾ��ϴ� �� �ƴѰ� ? OR�� ó��?

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
		//	UE_LOG(LogTemp, Warning, TEXT("LOCKONSTATE"));  // ���� Ǯ� ������ ��� ���� state��µ� .. ���� 
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
	/*OverlappingWeapon->ApplyDamage(���µȳ�, 1000, ENUM::holdattack)*/

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
	//���� ���� ���� ���� �����͸� ���� �迭 ����
	TArray<FHitResult> hitInfos;
	//���� ���� ����. ���� 750.0f
	FCollisionShape MySphere = FCollisionShape::MakeSphere(750.0f);
	//���� ������ ���� ����� ã�´�.
	bool bResult = GetWorld()->SweepMultiByObjectType(hitInfos, GetActorLocation(), GetActorLocation(), FQuat::Identity, ECC_WorldDynamic, MySphere);
	//����� ã�� ���
	if (bResult) {
		AActor* closestEnemy = nullptr;
		//���� �迭 ��ü�� Ž��
		for (auto& Hit : hitInfos) {
			//HitResult �������̽��� ������ �ִ� = ���̴�. �������̽��� ���� �ִ��� Ȯ��.
			IHitInterface* HitInterface = Cast<IHitInterface>(Hit.GetActor());
			//HitResult �������̽��� ������ ������
			if (HitInterface) {
				//�÷��̾�� ������ �Ÿ��� ���
				FVector hitLocation =  Hit.GetActor()->GetActorLocation();
				float betweenDistance = (GetActorLocation() - hitLocation).Length();
				//�̹� ���´���� �� ���� �ִٸ�, �÷��̾�� �Ÿ��� �� ª�� ����� ���´������ ��ü
				if (betweenDistance < lockOnEnemyDistance) {
					lockOnEnemyDistance = betweenDistance;
					closestEnemy = Hit.GetActor();
				}
			}
		}

		if (closestEnemy) {
			//���� ����� ������ ��� ����� ��ü ����
			//DrawDebugSphere(GetWorld(), GetActorLocation(), 750.0f, 100, FColor::Green, false, 3.0f);
			GetWorld()->GetFirstPlayerController()->SetIgnoreLookInput(true);
			GetCharacterMovement()->bOrientRotationToMovement = false;
			GetCharacterMovement()->bUseControllerDesiredRotation = true;
			bIsLockOn = true;

			CharacterState = ECharacterState::ECS_LockOn;

			// ���� ui �۾�
			// hit �������̽��� lockon �Լ� ȣ�� �� bool value�� true�� ���� ����(ui�� ü�¹�)
			// false�� ���� ������ ����� true�� ~ 

			IHitInterface* HitInterface = Cast<IHitInterface>(closestEnemy);
			HitInterface->Lockon(true);

			return closestEnemy;
		}
// 		else {
// 			//���Ǿ� Ʈ���̽� ����� �־����� ���� ����� ������ ����� ����� ��ü ����
// 			//DrawDebugSphere(GetWorld(), GetActorLocation(), 750.0f, 100, FColor::Yellow, false, 3.0f);
// 		}
	}
// 	else {
// 		//���Ǿ� Ʈ���̽� ����� ������ ���� ����� ��ü ����
// 		//DrawDebugSphere(GetWorld(), GetActorLocation(), 750.0f, 100, FColor::Red, false, 3.0f);
// 	}
	return nullptr;
}

void ABloodBornCharacter::DisEngageLockOn()
{
	// ���� ui �۾�
	// false�� ���� ������
	// ���⼭ hitinterface�� lockon �Լ��� bool false���ڸ� �������� �ؿ� 
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
	//���µ� ���ʹ̰� �ְ�,
	if (LockOnEnemyREF != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("11111"));
		//�Ͽµ� ����� �������̽��� ����. �������̽��� �ִ� ���͸� �ϿµǹǷ� ����ڵ�� ¥�� �ʾҴ�.
		IHitInterface* HitInterface = Cast<IHitInterface>(LockOnEnemyREF);

		//���µ� ���� ���ϻ����̰�, �Ÿ��� 150.0f�� ���
		if (HitInterface->GetInStun() && lockOnEnemyDistance < 150.0f) {
			UE_LOG(LogTemp, Warning, TEXT("22222"));

			//������� ����ڵ�
			if (EquippedWeaponSaw != nullptr) {
				UE_LOG(LogTemp, Warning, TEXT("33333"));

				//���⼭�� �ִϸ��̼Ǹ� �����Ű��, ���� ����� ó���� �ִϸ��̼� �������Ʈ���� �ϱ�
				//����̱� ����Ÿ��
				attackType = 2;
				if (ActionState == EActionState::EAS_Unoccupied)
				{
					// EquippedWeaponSaw->SetActorHiddenInGame(true);  // ���� �������� ���� ��Ƽ���� �κп��� applydamageó��
					PlayHoldAttackMontage();
					// ApplyDamage(LockOnEnemyREF, 500, EAttackType::HoldAttack);
					ActionState = EActionState::EAS_HoldAttacking;
				}

				//����̱Ⱑ �ߵ��� ��� �Ϲݰ����� �������� �ʰ� �Լ� ����
				// return;
			}
		}
	}

	//����̱� ������ �޼����� ������ �Ϲݰ��� ����� ����
	if (!HasEnoughStamina(Attributes->GetNormalAttackCost())) return;
	else
	{
		attackType = 0;
		// if (CanAttack()) �׸��� ĳ���� �����  bool CanAttack(); �����ϰ� cpp �� �����ؾ��� ���� �������� ���� �� �����ϸ� �� �ڵ� �ּ����� �ۼ�
		// {PlayN~ EAS_Normal ���� }
		// bool ABloodBornCharacter::CanAttack(){return 	ActionState == EActionState::EAS_Unoccupied && CharacterState != EcharacterState::ECS_Unequipped;}
		if (ActionState == EActionState::EAS_Unoccupied)   // �Ǽ� ���� ���Ϸ��� �ּ����� ����
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


		AnimInstance->Montage_Play(NormalAttackMontage);  // ������ �Ķ���͵��� �⺻�������� �ϴ� �ҰŶ� �� ��
		//const int32 Selection = FMath::RandRange(0, 2);

		FName SectionName = FName();  // FName ��� ����
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

// ������ ����� ������ �����丵�Լ��� �� ������ ���� ��
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
				UGameplayStatics::PlaySound2D(GetWorld(), hitSound1); // 30%�� Ȯ���� hitSound1 ���
			}
			else if (RandomNumber < 40) {
				UGameplayStatics::PlaySound2D(GetWorld(), hitSound2); // 10%�� Ȯ���� hitSound2 ���
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

		GetCharacterMovement()->MaxWalkSpeed = 500.0f;  // �̰� �� �ִ°��� 
		UE_LOG(LogTemp, Warning, TEXT("Player Hit! : %f"), DamageAmount);
		ActionState = EActionState::EAS_HitReaction;
		LastHitTime = GetWorld()->GetTimeSeconds();  // ������ ������ ���� �ð� ������Ʈ
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
		// ��Ÿ�� ��� ����
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
	 //DisableInput(Ge); ī�޶� ������ ���߱�?
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
	// return Attributes && Attributes->GetStamina() > Attributes->GetDodgeCost();  // �ۼ�Ʈ�� �ƴ϶� ���� ���׹̳� �ʿ�, ������ dodgecost�� �ϴ� �׽�Ʈ 
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
			//�Ͽµ� ���� �������̽��� ����
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

//�޴�ǥ�� ��ư�� ������ �޴��� ǥ�õȴ�. �޴��� ǥ�õǸ� ���콺 ���� �̿��� �Է��� ������.
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
// 			// Niagara �ý���
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



//void adf����̱�()
//{
//	if (bIsLockOn && ���� ���� ���� &&�Ÿ�)
//	{
//	 ����̱�
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
			//����̱� ��Ȳ���� ������ ��ü ȸ��
			RegainedHealth = Attributes->RegainableHealth;

		}

		// Attributes->RegainHealth(RegainedHealth);
		// ȸ�� ������ �ִ� ü���� ���
		//float MaxRegainableHealth = CalculateMaxRegainableHealth(Damage);

		//float RegainableHealth = Damage;

		//// ȸ�� ������ ü���� �ִ� ��쿡�� ȸ�� ����
		//if (RegainableHealth > 0.0f)
		//{
			// ȸ�� ������ ü���� �÷��̾�� ����
		Attributes->RegainHealth(RegainedHealth);
		// ȸ�� ���̾ư���
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
//	// ���� ü�°� ���� �������� ���� ���� ȸ�� ������ �ִ� ü��
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