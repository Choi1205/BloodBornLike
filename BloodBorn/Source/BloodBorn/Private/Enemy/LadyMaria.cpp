

#include "Enemy/LadyMaria.h"
#include "Enemy/LadyMariaAIController.h"
#include "../BloodBornCharacter.h"
#include "Enemy/BulletActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Enemy/SmokeFXActor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Enemy/BGMActor.h"
#include "Engine.h"

ALadyMaria::ALadyMaria()
{
	PrimaryActorTick.bCanEverTick = true;

	hair = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("hair"));
	hair->SetupAttachment(GetMesh(), TEXT("HairSocket"));
	hair->SetCollisionProfileName(TEXT("NoCollision"));
	hair->SetVisibility(true);

	rightSword1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightSword1"));
	rightSword1->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));
	rightSword1->SetCollisionProfileName(TEXT("NoCollision"));
	rightSword1->SetVisibility(true);

	rightTwoSword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightTwoSword"));
	rightTwoSword->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));
	rightTwoSword->SetCollisionProfileName(TEXT("NoCollision"));
	rightTwoSword->SetVisibility(false);

	rightSword2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightSword2"));
	rightSword2->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));
	rightSword2->SetCollisionProfileName(TEXT("NoCollision"));
	rightSword2->SetVisibility(false);

	rightSword3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightSword3"));
	rightSword3->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));
	rightSword3->SetCollisionProfileName(TEXT("NoCollision"));
	rightSword3->SetVisibility(false);

	rightDamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightDamageBox"));
	rightDamageCollision->SetupAttachment(rightSword1);
	rightDamageCollision->SetRelativeLocation(FVector(0.0f, 47.0f, 1.5f));
	rightDamageCollision->SetBoxExtent(FVector(1.0f, 36.0f, 3.0f));

	leftSword1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftSword1"));
	leftSword1->SetupAttachment(GetMesh(), TEXT("LeftHandSocket"));
	leftSword1->SetCollisionProfileName(TEXT("NoCollision"));
	leftSword1->SetVisibility(true);

	leftSword2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftSword2"));
	leftSword2->SetupAttachment(GetMesh(), TEXT("LeftHandSocket"));
	leftSword2->SetCollisionProfileName(TEXT("NoCollision"));
	leftSword2->SetVisibility(false);

	leftDamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftDamageBox"));
	leftDamageCollision->SetupAttachment(leftSword1);
	leftDamageCollision->SetRelativeLocation(FVector(0.0f, 28.0f, 0.0f));
	leftDamageCollision->SetBoxExtent(FVector(1.0f, 21.0f, 2.0f));

	gun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
	gun->SetupAttachment(GetMesh(), TEXT("LeftHandSocket"));
	gun->SetCollisionProfileName(TEXT("NoCollision"));
	gun->SetVisibility(false);

	bulletFirePoint = CreateDefaultSubobject<USceneComponent>(TEXT("BulletFirePoint"));
	bulletFirePoint->SetupAttachment(gun);
	bulletFirePoint->SetRelativeLocation(FVector(0.0f, -1.25f, -40.0f));

	GetCharacterMovement()->MaxWalkSpeed = 100.0f;
	GetCharacterMovement()->MaxAcceleration = 200.0f;

	rightEffect_V = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RightEffect"));
	rightEffect_V->SetupAttachment(rightSword1);
	rightEffect_V->SetRelativeLocation(FVector(0.0f, 90.0f, 0.0f));
	rightEffect_V->SetRelativeRotation(FRotator(-90.0f, 90.0f, 0.0f));
	rightEffect_V->bAutoActivate = false;

	rightEffect_H = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LefttEffect"));
	rightEffect_H->SetupAttachment(rightSword1);
	rightEffect_H->SetRelativeLocation(FVector(0.0f, 90.0f, 0.0f));
	rightEffect_H->SetRelativeRotation(FRotator(-90.0f, 90.0f, 90.0f));
	rightEffect_H->bAutoActivate = false;
}

void ALadyMaria::BeginPlay()
{
	Super::BeginPlay();

	mariaAI = Cast<ALadyMariaAIController>(GetController());
	AnimInstance = GetMesh()->GetAnimInstance();

	rightDamageCollision->OnComponentBeginOverlap.AddDynamic(this, &ALadyMaria::OnDealDamageOverlapBegin);
	leftDamageCollision->OnComponentBeginOverlap.AddDynamic(this, &ALadyMaria::OnDealDamageOverlapBegin);

	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	smokeActor1 = GetWorld()->SpawnActor<ASmokeFXActor>(smokeVFX, FVector::ZeroVector, FRotator::ZeroRotator, params);
	smokeActor2 = GetWorld()->SpawnActor<ASmokeFXActor>(smokeVFX, FVector::ZeroVector, FRotator::ZeroRotator, params);

	playerREF = FindPlayer_BP();

	bgmInstance = GetWorld()->SpawnActor<ABGMActor>(bgmActor, FVector::ZeroVector, FRotator::ZeroRotator, params);
	/*

	//źȯ�� �߻��Ѵ�
	ABulletActor* bullet = GetWorld()->SpawnActor<ABulletActor>(bulletFactory, bulletFirePoint->GetComponentLocation(), (playerREF->GetActorLocation() - bulletFirePoint->GetComponentLocation()).Rotation(), params);*/
}

void ALadyMaria::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//��� �÷��̿��� ��ã�� ��찡 �ֳ�...?
	if (playerREF != nullptr) {
		if (mariaAI != nullptr) {
			//�� ƽ���� �÷��̾���� �Ÿ��� ���
			distanceToPlayer = FVector::Distance(GetActorLocation(), playerREF->GetActorLocation());
			//�÷��̾��� �ӵ��� ���
			playerSpeed = playerREF->GetVelocity().Length();

			if (phaseState == EPhaseState::PHASE1 && mariaAI->bIsChangeMode) {
				if (mariaAI->bIsDualSword) {
					leftSword1->SetVisibility(true);
					gun->SetVisibility(false);
					rightTwoSword->SetVisibility(false);
				}
				else {
					leftSword1->SetVisibility(false);
					gun->SetVisibility(true);
					rightTwoSword->SetVisibility(true);
				}
				mariaAI->bIsChangeMode = false;
			}

			//�������� �ƴ϶�� ������ �׻� �÷��̾ �ٶ󺻴�.
			if (mariaAI->attackState == EAttackState::IDLE) {
				FRotator toward = (playerREF->GetActorLocation() - GetActorLocation()).Rotation();
				SetActorRotation(toward);

				//�ൿ�� ���߸� ���׹̳ʰ� ȸ���ȴ�.
				if (stamina < 1000.0f) {
					//�ִ�ġ���� ������ ȸ��. ����� ���� ȸ���� ��ȭ
					stamina += DeltaTime * staminaRegain;
				}
				else if (stamina > 1000.0f) {
					//�ִ�ġ�� ������ �ִ�ġ�� �����.
					stamina = 1000.0f;
				}

			}

			if (mariaAI->attackState == EAttackState::ASSULT) {
				Assult();
			}

			if (mariaAI->attackState == EAttackState::QUICKTHRUST || mariaAI->attackState == EAttackState::CHARGETHRUST) {
				Thrust();
			}
			else if (mariaAI->attackState == EAttackState::JUMPATTACK) {
				JumpAttack();
			}
			else if (mariaAI->attackState == EAttackState::QUICKSLASH || mariaAI->attackState == EAttackState::STRONGSLASH) {
				Slash();
			}

			if (bIsMovingWhileAttack) {
				if (mariaAI->attackState == EAttackState::QUICKTHRUST) {
					SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * 800, true);
				}
				else if (mariaAI->attackState == EAttackState::JUMPATTACK) {
					temp += DeltaTime;
					FVector moveLoc = FMath::Lerp(GetActorLocation(), movePlace, 1 /(moveDeltaTime / temp));
					UE_LOG(LogTemp, Warning, TEXT("%f"), 1 / moveDeltaTime);
					SetActorLocation(moveLoc, true);
				}
				else if (mariaAI->attackState == EAttackState::ASSULT) {
					temp += DeltaTime;
					FVector moveLoc = FMath::Lerp(GetActorLocation(), movePlace, 1 /(assultDeltaTime / temp));
					SetActorLocation(moveLoc, true);
				}
				else {
					SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * 400, true);
				}
			}
			if (bIsAimmingWhileAttack) {
				SetActorRotation((playerREF->GetActorLocation() - GetActorLocation()).Rotation());
			}
		}
	}
	else if (findPlayerTimer < 0) {
		//���� ��ã������ �ٽ�ã��
		playerREF = FindPlayer_BP();
	}
	else {
		//ƽ �̺�Ʈ���� ��� GetAllActorsOfClass�� �����ϸ� ���ɿ� ������ �� ������ ����ǹǷ�, 5�� Ÿ�̸Ӹ� ������.
		findPlayerTimer -= DeltaTime;
	}
}

//����÷��̿��� ȣ��. �÷��̾� ã��
ABloodBornCharacter* ALadyMaria::FindPlayer_BP()
{
	for(TActorIterator<ABloodBornCharacter> iter(GetWorld()); iter; ++iter) {
		return *iter;
	}

	findPlayerTimer = 5.0f;
	return nullptr;
}

//�Ÿ� ���� �Լ�
float ALadyMaria::GetPlayerDistance()
{
	return distanceToPlayer;
}
//�ӵ� ���� �Լ�
float ALadyMaria::GetPlayerSpeed()
{
	return playerSpeed;
}
//���׹̳� ���� �Լ�
float ALadyMaria::GetBossStamina()
{
	return stamina;
}

void ALadyMaria::WalkToPlayer()
{
	FVector playerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector towardPlayer = playerLocation - GetActorLocation();

	if (distanceToPlayer > 150.0f) {
		AddMovementInput(towardPlayer.GetSafeNormal(1.0));
	}
	else {
		//150 �Ÿ� �����̸� �Ʒ� ��� �ӵ��� �÷��̾� �ֺ��� ����. ���� ���� �� ���������ϋ� �ֺ� �̵���
		FVector sidemoveDir = GetActorRightVector() * 5;
		if (mariaAI->bIsLeftMove) {
			sidemoveDir *= -1;
		}
		AddMovementInput(sidemoveDir.GetSafeNormal(1.0));
	}
}

void ALadyMaria::ForwardDodge()
{
	mariaAI->attackState = EAttackState::FORWARDDODGE;
	AnimInstance->Montage_Play(AnimDodgeForward);
}

void ALadyMaria::GetHit(const FVector& ImpactPoint)
{
	UE_LOG(LogTemp, Warning, TEXT("ImpactPoint"));

	// ���� �� Dot ���� ������ �� �ִϸ��̼ǵ� ���� ���߿� �ʿ����� �ƴ����� �𸣰���
	// �� ������ �ִϸ��̼��� ��� Theta ������ �� 0�ΰǰ� 
	const FVector Forward = GetActorForwardVector();
	// Lower Impact Point to the Enemy's Actor Location Z
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation().GetSafeNormal());

	// Forward * ToHit = | Forward | | ToHit | * cos(theta), | Forward | | ToHit | : normalized����
	// | Forward | = 1,  | ToHit | = 1, �׷��Ƿ� Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	// convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	}
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);  //Forward ��ֶ������ؼ� ���� 1�̶� 60 ����
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
}

float ALadyMaria::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount);
		UE_LOG(LogTemp, Warning, TEXT("Damage"));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Blue, FString::Printf(TEXT("%f"), DamageAmount));
		}
		// ���⿡ ü�¹�(?)
	}

	GotDamage(DamageAmount);

	return DamageAmount;
}

//��ݰ��� ������ ó���� �ʿ��ϱ� ������ ü���� �����ϴ� �Լ��� ���� �ۼ�
void ALadyMaria::GotDamage(float damage)
{
	//����̱� ���� ����߿��� �ٸ� �������� ���� �ʴ´�.
	if (!bIsHitHoldAttack) {
		//HP�� ������ ��ŭ ��´�
		healthPoint = FMath::Max(healthPoint - damage, 0.0f);
	}

	//�������� ������ ����� ������ �� üũ. ������ ���� ����� ��� ���⼭ üũ�ϰ�, ����� ����� ��� ��Ƽ���̷� �̵�?
	if (phaseState == EPhaseState::PHASE1 && healthPoint < phase2HP) {
		phaseState = EPhaseState::PHASE2;
		staminaRegain = 75.0f;
		weakAttack = 100;
		strongAttack = 150;
		rightSword1->SetVisibility(false);
		rightTwoSword->SetVisibility(false);
		rightSword2->SetVisibility(true);
		rightDamageCollision->SetBoxExtent(FVector(2.0f, 36.0f, 3.0f));
		leftSword1->SetVisibility(false);
		gun->SetVisibility(false);
		leftSword2->SetVisibility(true);
		leftDamageCollision->SetRelativeLocation(FVector(0.0f, 54.0f, -1.0f));
		leftDamageCollision->SetBoxExtent(FVector(2.0f, 46.0f, 3.0f));
	}
	else if (phaseState == EPhaseState::PHASE2 && healthPoint < phase3HP) {
		phaseState = EPhaseState::PHASE3;
		staminaRegain = 100.0f;
		weakAttack = 150;
		strongAttack = 200;
		rightSword2->SetVisibility(false);
		rightSword3->SetVisibility(true);
		rightDamageCollision->SetRelativeLocation(FVector(0.0f, 68.0f, 0.0f));
		rightDamageCollision->SetBoxExtent(FVector(2.0f, 60.0f, 4.5f));
	}
	//�ǰ� ����Ʈ ����
	instanceEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), hitEffect, GetActorLocation(), FRotator::ZeroRotator, FVector(3.0f));

	//AI���� ����ڵ�
	if (mariaAI != nullptr) {
		if (healthPoint == 0) {
			bIsCanDealDamage = false;
			mariaAI->attackState = EAttackState::DIED;
			if (AnimInstance->Montage_IsPlaying(NULL)) {
				AnimInstance->Montage_Stop(NULL);
			}
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			mariaAI->UnPossess();
			mariaAI = nullptr;
			bgmInstance->slowKillThis();
		}
		else {
			if (mariaAI->bIsStun && damage >= 500.0f && !bIsHitHoldAttack) {
				bIsHitHoldAttack = true;
			}
			else if (!mariaAI->bIsStun && !bIsSuperArmor) {
				bIsCanDealDamage = false;
				UE_LOG(LogTemp, Warning, TEXT("Hit!"));

				if (AnimInstance->Montage_IsPlaying(NULL)) {
					AnimInstance->Montage_Stop(NULL);
				}
				bIsMovingWhileAttack = false;
				mariaAI->attackState = EAttackState::TAKEHIT;
				bIsCanDealDamage = false;
				bIsAimmingWhileAttack = false;
				bIsMovingWhileAttack = false;
				AnimInstance->Montage_Play(AnimBossHit);
				AnimInstance->Montage_SetPlayRate(AnimBossHit, 1.0f);
			}
		}
	}
}

void ALadyMaria::GotParryAttackCPP(float damage)
{
	//��ݰ��� ȸ��üũ. ȸ���� 50%
	if (mariaAI->attackState == EAttackState::IDLE && mariaAI->RandomNextMoveTF(50)) {
		//������ ON���¿����� ����.
		mariaAI->bIsForwardDodge = false;
		//ȸ�� ���� ����
		if (mariaAI->RandomNextMoveTF(50)) {
			mariaAI->bIsLeftMove = true;
			AnimInstance->Montage_Play(AnimDodgeLeft);
		}
		else {
			mariaAI->bIsLeftMove = false;
			AnimInstance->Montage_Play(AnimDodgeRight);
		}
	}
	else {
		GotDamage(damage);
		
		if (mariaAI != nullptr) {
			if (bIsCanParryed) {
				if (AnimInstance->Montage_IsPlaying(NULL)) {
					AnimInstance->Montage_Stop(NULL);
				}
				bIsCanParryed = false;
				mariaAI->bIsStun = true;
			}
		}
		
		//�и����� �� ���������� ��
		UE_LOG(LogTemp, Warning, TEXT("Gun Attack Damage : %.0f"), damage);
	}
}

bool ALadyMaria::GetInStun()
{
	return mariaAI->bIsStun;
}

float ALadyMaria::GetHealth()
{
	return healthPoint;
}

void ALadyMaria::OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABloodBornCharacter* Player = Cast<ABloodBornCharacter>(OtherActor);//���� ���Ͱ� �÷��̾����� Ȯ��
	if (Player && bIsCanDealDamage) {//�÷��̾��̰�, ���ݵ� �����ϴٸ�
		//�÷��̾�� �������� �Ծ���.
		if (mariaAI->attackState == EAttackState::QUICKTHRUST) {
			UGameplayStatics::ApplyDamage(Player, strongAttack, GetInstigator()->GetController(), this, UDamageType::StaticClass());
		}
		else {
			UGameplayStatics::ApplyDamage(Player, weakAttack, GetInstigator()->GetController(), this, UDamageType::StaticClass());
		}
		bIsCanDealDamage = false;
	}
}

void ALadyMaria::Slash()
{
	//���� ������̴� ��Ÿ�ְ� ������
	if (AnimInstance->IsAnyMontagePlaying()) {
		//���� ���
		return;
	}

	//���׹̳� �Ҹ�
	stamina -= 50.0f;

	//������ ��������� ���
	if (mariaAI->attackState == EAttackState::QUICKSLASH) {
		AnimInstance->Montage_Play(AnimQuickSlash);
	}
	else {
		AnimInstance->Montage_Play(AnimStrongSlash);
	}
}

void ALadyMaria::Thrust()
{
	//���� ������̴� ��Ÿ�ְ� ������
	if (AnimInstance->IsAnyMontagePlaying()) {
		//���� ���
		return;
	}

	//������ 
	if (mariaAI->attackState == EAttackState::QUICKTHRUST) {
		//��⸦ ���
		AnimInstance->Montage_Play(AnimQuickThrust);
	}
	else {
		AnimInstance->Montage_Play(AnimChargeThrust);
	}
	//���׹̳� �Ҹ�
	stamina -= 100.0f;
}

void ALadyMaria::JumpAttack()
{
	//���� ������̴� ��Ÿ�ְ� ������
	if (AnimInstance->IsAnyMontagePlaying()) {
		//���� ���
		return;
	}

	//������ �������⸦ ���
	AnimInstance->Montage_Play(AnimJumpAttack);
	//���׹̳� �Ҹ�
	stamina -= 100.0f;
}

void ALadyMaria::Assult()
{
	//���� ������̴� ��Ÿ�ְ� ������
	if (AnimInstance->IsAnyMontagePlaying()) {
		//���� ���
		return;
	}

	//������ �������⸦ ���
	AnimInstance->Montage_Play(AnimAssult);
}

void ALadyMaria::AimGun()
{
	//��Ÿ������� �ߵ��ǹǷ�, �ൿ���� �ƴϰ�, ������� ��Ÿ�ְ� ���� ���(ȸ������ �ƴѰ��)
	if (mariaAI->attackState == EAttackState::IDLE && !AnimInstance->IsAnyMontagePlaying()) {
		mariaAI->attackState = EAttackState::AIMMINGGUN;
		AnimInstance->Montage_Play(AnimGunShot);
	}
}

void ALadyMaria::EffectMove()
{
	SetActorRotation((playerREF->GetActorLocation() - GetActorLocation()).Rotation());
	float aim = FMath::Min(distanceToPlayer + 25.0f, 1000.0f);
	rightEffect_V->SetRelativeLocation(FVector(0.0f, aim, 0.0f));
	rightEffect_H->SetRelativeLocation(FVector(0.0f, aim, 0.0f));
}

void ALadyMaria::EffectOn()
{
	if (mariaAI->attackState == EAttackState::QUICKTHRUST || mariaAI->attackState == EAttackState::CHARGETHRUST) {
		rightEffect_V->SetRelativeRotation(FRotator(0.0f, -90.0f, 180.0f));
		rightEffect_H->SetRelativeRotation(FRotator(0.0f, -90.0f, -90.0f));
	}
	/*else if (mariaAI->attackState == EAttackState::JUMPATTACK) {
		TArray<FHitResult> hitInfos;
		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(this);
		bool bResult = GetWorld()->SweepMultiByProfile(hitInfos, GetActorLocation(), GetActorLocation(), FQuat::Identity, FName(TEXT("Pawn")), FCollisionShape::MakeSphere(1000.0f), queryParams);
		if (bResult) {
			for (auto& hit : hitInfos) {
				UE_LOG(LogTemp, Warning, TEXT("%s"), *hit.GetActor()->GetActorNameOrLabel());
				ABloodBornCharacter* player = Cast<ABloodBornCharacter>(hit.GetActor());
				if (player != nullptr) {
					UGameplayStatics::ApplyDamage(player, strongAttack, GetInstigator()->GetController(), this, UDamageType::StaticClass());
					break;
				}
			}
		}
	}*/
	rightEffect_V->Activate(true);
	rightEffect_H->Activate(true);
}

void ALadyMaria::ABP_FireGun()
{
	
	//źȯ�� �߻�� ������ �ִԳ�Ƽ���̸� �̿��Ͽ� �ٽ� ����ȴ�.
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//źȯ�� �߻��Ѵ�
	ABulletActor* bullet = GetWorld()->SpawnActor<ABulletActor>(bulletFactory, bulletFirePoint->GetComponentLocation(), (playerREF->GetActorLocation() - bulletFirePoint->GetComponentLocation()).Rotation(), params);
	if (bullet != nullptr) {
		bullet->SetBulletSpeed(2000.0f);
		bullet->SetFirePower(40.0f);
	}
}

void ALadyMaria::ABP_2ndSlowEnd()
{
	bIsCanDealDamage = false;
	if (mariaAI->attackState == EAttackState::QUICKTHRUST || mariaAI->attackState == EAttackState::CHARGETHRUST) {
		//
		rightSword1->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		rightSword2->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		rightSword3->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		rightTwoSword->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		rightEffect_V->SetRelativeRotation(FRotator(-90.0f, 90.0f, 0.0f));
		rightEffect_H->SetRelativeRotation(FRotator(-90.0f, 90.0f, 90.0f));

		if (mariaAI->attackState == EAttackState::CHARGETHRUST) {
			AnimInstance->Montage_SetPlayRate(AnimChargeThrust, 1.0f);
			TArray<FHitResult> hitInfos;
			FVector startLoc = GetActorLocation();
			FVector endLoc = startLoc + GetActorForwardVector() * 1000.0f;
			FCollisionQueryParams queryParams;
			queryParams.AddIgnoredActor(this);
			bool bResult = GetWorld()->SweepMultiByProfile(hitInfos, startLoc, endLoc, FQuat::Identity, FName(TEXT("Pawn")), FCollisionShape::MakeSphere(17.0f), queryParams);
			if (bResult) {
				for (auto& hit : hitInfos) {
					UE_LOG(LogTemp, Warning, TEXT("%s"), *hit.GetActor()->GetActorNameOrLabel());
					ABloodBornCharacter* player = Cast<ABloodBornCharacter>(hit.GetActor());
					if (player != nullptr) {
						UGameplayStatics::ApplyDamage(player, strongAttack, GetInstigator()->GetController(), this, UDamageType::StaticClass());
						break;
					}
				}
			}
			for (int32 i = 1; i <= 20; i++) {
				//50���� 1000����
				FVector effectLoc = startLoc + GetActorForwardVector() * 50.0f * i;
				FRotator effectRot = GetActorForwardVector().Rotation();
				instanceEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), bloodThrustEffect, effectLoc, effectRot, FVector(3.0f));
			}
		}
		else {
			AnimInstance->Montage_SetPlayRate(AnimQuickThrust, 1.0f);
		}
	}
	else {
		rightEffect_V->SetRelativeLocation(FVector(0.0f, 90.0f, 0.0f));
		rightEffect_H->SetRelativeLocation(FVector(0.0f, 90.0f, 0.0f));
	}
	
}

void ALadyMaria::ABP_DodgeEnd()
{
	if(mariaAI->bIsForwardDodge) {
		FVector playerToMaria = (GetActorLocation() - playerREF->GetActorLocation()).GetSafeNormal() * 100.0f;
		SetActorLocation(playerREF->GetActorLocation() + playerToMaria);
		mariaAI->bIsForwardDodge = false;
	}
	else{
		FVector nextLoc = GetActorRightVector();
		if(mariaAI->bIsLeftMove){
			nextLoc *= -1.0f;
		}
		nextLoc = GetActorLocation() + nextLoc * 200.0f;

		SetActorLocation(nextLoc);
		AnimInstance->Montage_SetPlayRate(NULL, 0.25f);
	}

	GetMesh()->SetVisibility(true, true);

	switch (phaseState) {
	case EPhaseState::PHASE1:
		if (mariaAI->bIsDualSword) {
			rightTwoSword->SetVisibility(false);
			rightSword2->SetVisibility(false);
			rightSword3->SetVisibility(false);
			leftSword2->SetVisibility(false);
			gun->SetVisibility(false);
		}
		else {
			rightSword1->SetVisibility(false);
			rightSword2->SetVisibility(false);
			rightSword3->SetVisibility(false);
			leftSword1->SetVisibility(false);
			leftSword2->SetVisibility(false);
		}
		break;
	case EPhaseState::PHASE2:
		rightSword1->SetVisibility(false);
		rightTwoSword->SetVisibility(false);
		rightSword3->SetVisibility(false);
		leftSword1->SetVisibility(false);
		gun->SetVisibility(false);
		break;
	case EPhaseState::PHASE3:
		rightSword1->SetVisibility(false);
		rightTwoSword->SetVisibility(false);
		rightSword2->SetVisibility(false);
		leftSword1->SetVisibility(false);
		gun->SetVisibility(false);
		break;
	}

	if (smokeActor2 != nullptr) {
		smokeActor2->SetActorLocation(GetActorLocation());
		smokeActor2->PlayFX();
	}
	bIsSuperArmor = false;
	mariaAI->attackState = EAttackState::IDLE;
}

void ALadyMaria::ABP_BossHitEnd()
{
	mariaAI->attackState = EAttackState::IDLE;
}

void ALadyMaria::ABP_BossJumpTop()
{
	//�÷��̾� ��ġ�� �̿�, ������ġ ���
	//�÷��̾� ��ġ�� ���� ����.
	movePlace = playerREF->GetActorLocation() - playerREF->GetActorForwardVector() * 100.0f;
	temp = 0.0f;
}

void ALadyMaria::ABP_AssultChargeEnd()
{
	//�÷��̾�� 1/3�Ÿ� + �������� 500.0f��ġ�� �̵�
	FVector towardPlayer = playerREF->GetActorLocation() - GetActorLocation();

	movePlace = GetActorLocation() + towardPlayer * 2.0f / 3.0f + GetActorRightVector() * -900.0f;
	temp = 0.0f;
}

void ALadyMaria::ABP_AssultDodgeEnd()
{
	movePlace = playerREF->GetActorLocation() - playerREF->GetActorForwardVector() * 100.0f;
	EffectOn();
}
