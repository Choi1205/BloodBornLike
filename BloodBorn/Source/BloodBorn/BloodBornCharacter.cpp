// Copyright Epic Games, Inc. All Rights Reserved.

#include "BloodBornCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Animation/AnimMontage.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include <Components/AttributeComponent.h>

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ABloodBornCharacter

ABloodBornCharacter::ABloodBornCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// sawMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Saw Mesh Comp"));
	// sawMeshComp->SetupAttachment(GetMesh());
	// ConstructorHelpers::FObjectFinder<USkeletalMesh> TempSawMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/76MJ/Character/Weapon/saw-cleaver-bloodborne/source/SK_WeaponSaw.SK_WeaponSaw'"));
	// if (TempSawMesh.Succeeded())
	// {
	//	sawMeshComp->SetSkeletalMesh((TempSawMesh.Object))-;
	// }

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

}

void ABloodBornCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetSawBox())
	{
		EquippedWeapon->GetSawBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}



void ABloodBornCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	//FName WeaponSocket(TEXT("RightHandSocket"));
	//auto CurWeapon = GetWorld()->SpawnActor<AWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	//if (nullptr != CurWeapon)
	//{
	//	CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	//}
	//FName WeaponSocket(TEXT("RightHandSocket"));
	//auto CurWeapon = GetWorld()->SpawnActor<AWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	//if (nullptr != CurWeapon)
	//{
		// Get the socket transform
	//	FTransform WeaponSocketTransform = GetMesh()->GetSocketTransform(WeaponSocket);
	//	FVector WeaponScale = FVector(0.005f, 0.005f, 0.005f); // 예시로 크기를 줄이는 벡터를 생성합니다.
	//	CurWeapon->SetActorScale3D(WeaponScale);

		// Set the weapon's location and rotation to match the socket's
	//	CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	//	CurWeapon->SetActorRelativeLocation(WeaponSocketTransform.GetLocation());
	//	CurWeapon->SetActorRelativeRotation(WeaponSocketTransform.GetRotation());
	//}
}



//////////////////////////////////////////////////////////////////////////
// Input



void ABloodBornCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABloodBornCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABloodBornCharacter::Look);

		// Normal Attacking
		EnhancedInputComponent->BindAction(NormalAttackAction, ETriggerEvent::Triggered, this, &ABloodBornCharacter::NormalAttack);

		EnhancedInputComponent->BindAction(StrongAttackAction, ETriggerEvent::Triggered, this, &ABloodBornCharacter::StrongAttack);

		// Equip
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ABloodBornCharacter::Equip);

		//Dodge
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &ABloodBornCharacter::Dodge);

		EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Started, this, &ABloodBornCharacter::LockOn);



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

	if (ActionState == EActionState::EAS_StrongAttacking) return;	 // 공격할 때 못 움직이게 강제, 근데 이렇게 하면 강공격 때도 또 받아야하는 거 아닌가 ? OR로 처리?

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
	}
}

void ABloodBornCharacter::Look(const FInputActionValue& Value)
{
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

void ABloodBornCharacter::Equip()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);  //어라 왜 equip 이 두 번. ..... . .... void~ Equip 을 FPressed로 바꿀까...
		OverlappingWeapon->SetOwner(this);
		//GetOwner()
		OverlappingWeapon->SetInstigator(this);
		//CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		OverlappingItem = nullptr;
		EquippedWeapon = OverlappingWeapon;
	}
	//else
	//{
	//	if (ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped)
	//	{
	//	}
	//}
}

void ABloodBornCharacter::Dodge()
{
	//Selection = 0;
	if (ActionState == EActionState::EAS_Unoccupied)
	{
		GetCharacterMovement()->MaxWalkSpeed = 1000.f;
		PlayDodgeMontage();
		ActionState = EActionState::EAS_Dodge;
	}

}



// 흠 필요한가? 
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

void ABloodBornCharacter::NormalAttack()
{
	// if (CanAttack()) 그리고 캐릭터 헤더에  bool CanAttack(); 선언하고 cpp 에 정의해야함 선언 안했지만 선언 후 정의하면 쓸 코드 주석으로 작성
	// {PlayN~ EAS_Normal 고대로 }
	// bool ABloodBornCharacter::CanAttack(){return 	ActionState == EActionState::EAS_Unoccupied && CharacterState != EcharacterState::ECS_Unequipped;}
	if (ActionState == EActionState::EAS_Unoccupied)   // 맨손 공격 안하려면 주석으로 쓰기
	{
		GetCharacterMovement()->MaxWalkSpeed = 100.f;
		PlayNormalAttackMontage();
		ActionState = EActionState::EAS_NormalAttacking;
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
	if (ActionState == EActionState::EAS_Unoccupied)
	{
		PlayStrongAttackMontage();
		ActionState = EActionState::EAS_StrongAttacking;
	}
}

void ABloodBornCharacter::PlayStrongAttackMontage()
{
	PlayMontageSection(StrongAttackMontage, FName("HeavyAttack"));
	MakeNoise(1.0f);
}

void ABloodBornCharacter::AttackEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	ActionState = EActionState::EAS_Unoccupied;
}

void ABloodBornCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

float ABloodBornCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	health -= DamageAmount;

	if (health > 0 && !bIsInvincible && Attributes)  //->GetHealthPercent() > 0.f   흠 걍 isAlive 로 만들까 아 takedamage 말고도 그냥 gethit를 만들든가 해야지 이게 무슨 괴상한 ,,
	{
		Attributes->ReceiveDamage(DamageAmount);
		PlayHitReactMontage();
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		UE_LOG(LogTemp, Warning, TEXT("Player Hit! : %f"), DamageAmount);
		ActionState = EActionState::EAS_HitReaction;
	}
	else if (health == 0)
	{
		PlayDeathMontage();
	}
	return DamageAmount;
}

void ABloodBornCharacter::PlayHitReactMontage()
{
	PlayMontageSection(HitReactMontage, FName("HitReact"));
}

//void ABloodBornCharacter::Die()
//{

	// GetMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//}

void ABloodBornCharacter::PlayDeathMontage()
{
	PlayMontageSection(DeathMontage, FName("Death"));
	ActionState = EActionState::EAS_Dead;
}

void ABloodBornCharacter::LockOn()
{
	CharacterState = ECharacterState::ECS_LockOn;
}
