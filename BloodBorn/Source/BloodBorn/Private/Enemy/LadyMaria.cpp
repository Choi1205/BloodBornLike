//보스 캐릭터 마리아의 코드 AI컨트롤러와 세트

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
#include "Components/WidgetComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Enemy/SmokeFXActor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Enemy/BGMActor.h"
#include "Engine.h"
#include "Enemy/LadyMariaJumpEffectActor.h"
#include "Enemy/BloodDecalActor.h"
#include "Enemy/EnemyHPWidget.h"


ALadyMaria::ALadyMaria()
{
	PrimaryActorTick.bCanEverTick = true;

	hair = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("hair"));
	hair->SetupAttachment(GetMesh(), TEXT("HairSocket"));
	hair->SetCollisionProfileName(TEXT("NoCollision"));
	hair->SetVisibility(true);

	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetWorldScale3D(FVector(1.3f));

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

	deadCloud = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DeadCloud"));
	deadCloud->SetupAttachment(GetMesh(), TEXT("Hips"));
	deadCloud->bAutoActivate = false;

	floatingWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("floatingWidgetComp"));
	floatingWidgetComp->SetupAttachment(GetMesh(), FName("Spine"));
	floatingWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	floatingWidgetComp->SetVisibility(false);
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
	
	jumpEffectInstance = GetWorld()->SpawnActor<ALadyMariaJumpEffectActor>(jumpEffect, FVector::ZeroVector, FRotator::ZeroRotator, params);

	FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, true);
	
	jumpEffectInstance->AttachToComponent(GetMesh(), rules, FName("Hips"));
	jumpEffectInstance->SetActorRotation(FRotator::ZeroRotator);

	/*
	if (pauseWidget_bp != nullptr) {
		pauseWidget = CreateWidget<UPauseWidget>(GetWorld(), pauseWidget_bp);
		if (pauseWidget != nullptr) {
			pauseWidget->AddToViewport();
			pauseWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	*/
	if (bossWidget_bp != nullptr) {
		bossWidget = CreateWidget<UEnemyHPWidget>(GetWorld(), bossWidget_bp);
		if (bossWidget != nullptr) {
			bossWidget->AddToViewport();
			bossWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ALadyMaria::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//비긴 플레이에서 못찾는 경우가 있나...?
	if (playerREF != nullptr) {
		if (mariaAI != nullptr) {
			//매 틱마다 플레이어와의 거리를 잰다
			distanceToPlayer = FVector::Distance(GetActorLocation(), playerREF->GetActorLocation());
			//플레이어의 속도도 잰다
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

			//공격중이 아니라면 보스는 항상 플레이어를 바라본다.
			if (mariaAI->attackState == EAttackState::IDLE) {
				FRotator toward = (playerREF->GetActorLocation() - GetActorLocation()).Rotation();
				toward.Pitch = 0.0f;
				SetActorRotation(toward);

				//행동을 멈추면 스테미너가 회복된다.
				if (stamina < 1000.0f) {
					//최대치보다 적으면 회복. 페이즈에 따라 회복량 변화
					stamina += DeltaTime * staminaRegain;
				}
				else if (stamina > 1000.0f) {
					//최대치를 넘으면 최대치로 맞춘다.
					stamina = 1000.0f;
				}

			}

			if (mariaAI->attackState == EAttackState::ASSULT) {
				Assult();
			}
			else if (mariaAI->attackState == EAttackState::AIMMINGGUN) {
				AimGun();
			}
			else if (mariaAI->attackState == EAttackState::QUICKTHRUST || mariaAI->attackState == EAttackState::CHARGETHRUST) {
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
					FVector moveLoc = FMath::Lerp(GetActorLocation(), movePlace, temp/ moveDeltaTime);
					UE_LOG(LogTemp, Warning, TEXT("%f"), 1 / moveDeltaTime);
					SetActorLocation(moveLoc);
				}
				else if (mariaAI->attackState == EAttackState::ASSULT) {
					temp += DeltaTime;
					if (assultMoving) {
						FVector moveLoc = FMath::Lerp(GetActorLocation(), movePlace, temp / assultMovingTime);
						SetActorLocation(moveLoc, true);
					}
					else {
						FVector moveLoc = FMath::Lerp(GetActorLocation(), movePlace, temp / assultAttackTime);
						SetActorLocation(moveLoc, true);
					}
				}
				else {
					SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * 400, true);
				}
			}

			if (bIsAimmingWhileAttack && mariaAI->attackState != EAttackState::JUMPATTACK) {
				FRotator newRot = (playerREF->GetActorLocation() - GetActorLocation()).Rotation();
				newRot.Pitch = 0.0f;
				SetActorRotation(newRot);
			}
		}
	}
	else if (findPlayerTimer < 0) {
		//만약 못찾았으면 다시찾기
		playerREF = FindPlayer_BP();
	}
	else {
		//틱 이벤트에서 계속 GetAllActorsOfClass를 실행하면 성능에 영향을 줄 것으로 예상되므로, 5초 타이머를 돌린다.
		findPlayerTimer -= DeltaTime;
	}
}

//비긴플레이에서 호출. 플레이어 찾기
ABloodBornCharacter* ALadyMaria::FindPlayer_BP()
{
	for(TActorIterator<ABloodBornCharacter> iter(GetWorld()); iter; ++iter) {
		return *iter;
	}

	findPlayerTimer = 5.0f;
	return nullptr;
}

//거리 게터 함수
float ALadyMaria::GetPlayerDistance()
{
	return distanceToPlayer;
}
//속도 게터 함수
float ALadyMaria::GetPlayerSpeed()
{
	return playerSpeed;
}
//스테미나 게터 함수
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
		//150 거리 안쪽이면 아래 상수 속도로 플레이어 주변을 돈다. 공격 동료 후 근접상태일떄 주변 이동용
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
	int32 num = FMath::RandRange(0, 2);
	switch (num) {
	case 0:
		UGameplayStatics::PlaySound2D(GetWorld(), dodgeSound1);
		break;
	case 1:
		UGameplayStatics::PlaySound2D(GetWorld(), dodgeSound2);
		break;
	case 2:
		UGameplayStatics::PlaySound2D(GetWorld(), dodgeSound3);
		break;
	}
}

void ALadyMaria::GetHit(const FVector& ImpactPoint)
{
	UE_LOG(LogTemp, Warning, TEXT("ImpactPoint"));

	// 여긴 걍 Dot 관련 지금은 적 애니메이션도 없고 나중에 필요할지 아닐지도 모르겠음
	// 흠 지금은 애니메이션이 없어서 Theta 각도가 다 0인건가 
	const FVector Forward = GetActorForwardVector();
	// Lower Impact Point to the Enemy's Actor Location Z
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation().GetSafeNormal());

	// Forward * ToHit = | Forward | | ToHit | * cos(theta), | Forward | | ToHit | : normalized벡터
	// | Forward | = 1,  | ToHit | = 1, 그러므로 Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	// convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	}
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);  //Forward 노멀라이즈해서 길이 1이라 60 곱함
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
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
		// 여기에 체력바(?)
	}

	GotDamage(DamageAmount);

	return DamageAmount;
}

//사격공격 데미지 처리도 필요하기 때문에 체력이 감소하는 함수를 따로 작성
void ALadyMaria::GotDamage(float damage)
{
	//내장뽑기 공격 모션중에는 다른 데미지를 받지 않는다.
	if (bIsHitHoldAttack) {
		//HP를 데미지 만큼 깍는다
		healthPoint = FMath::Max(healthPoint - damage, 0.0f);
	}

	bossWidget->SetHealthBar(healthPoint / maxHealthPoint, damage);

	//데미지를 입으면 페이즈를 변경할 지 체크. 페이즈 변경 모션이 없어서 여기서 체크하고, 모션이 생기면 모션 노티파이로 이동?
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
		rightSword2->SetVisibility(false);
		rightSword3->SetVisibility(true);
		rightDamageCollision->SetRelativeLocation(FVector(0.0f, 68.0f, 0.0f));
		rightDamageCollision->SetBoxExtent(FVector(2.0f, 60.0f, 4.5f));
		rightEffect_V->SetAsset(phase3Effect);
		rightEffect_H->SetAsset(phase3Effect);
	}
	//피격 이펙트 생성
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), hitEffect, GetActorLocation(), FRotator::ZeroRotator, FVector(3.0f));

	//AI제거 방어코드
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

			if (damage >= 500) {
				UGameplayStatics::PlaySound2D(GetWorld(), hitHoldAttackSound);
			}
			if (FMath::RandRange(0, 1) == 0) {
				UGameplayStatics::PlaySound2D(GetWorld(), dieSound1);
			}
			else {
				UGameplayStatics::PlaySound2D(GetWorld(), dieSound2);
			}
			UGameplayStatics::PlaySound2D(GetWorld(), dieVoice);
			deadCloud->Activate();
		}
		else {
			if (mariaAI->bIsStun && damage >= 500.0f && bIsHitHoldAttack) {
				bIsHitHoldAttack = false;
				MakeBloodDecal(GetActorLocation(), false);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HoldAttackHitEffect, GetActorLocation(), FRotator::ZeroRotator, FVector(3.0f));
				UGameplayStatics::PlaySound2D(GetWorld(), hitHoldAttackSound);
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
				if (FMath::RandRange(0, 1) == 0) {
					UGameplayStatics::PlaySound2D(GetWorld(), hitSound1);
				}
				else {
					UGameplayStatics::PlaySound2D(GetWorld(), hitSound2);
				}
			}
		}
	}
}

void ALadyMaria::GotParryAttackCPP(float damage)
{
	//사격공격 회피체크. 회피율 50%
	if (mariaAI->attackState == EAttackState::IDLE && mariaAI->RandomNextMoveTF(50)) {
		//전방대시 ON상태였으면 끈다.
		mariaAI->bIsForwardDodge = false;
		//회피 방향 판정
		if (mariaAI->RandomNextMoveTF(50)) {
			mariaAI->bIsLeftMove = true;
			AnimInstance->Montage_Play(AnimDodgeLeft);
		}
		else {
			mariaAI->bIsLeftMove = false;
			AnimInstance->Montage_Play(AnimDodgeRight);
		}
		int32 num = FMath::RandRange(0, 2);
		switch (num) {
		case 0:
			UGameplayStatics::PlaySound2D(GetWorld(), dodgeSound1);
			break;
		case 1:
			UGameplayStatics::PlaySound2D(GetWorld(), dodgeSound2);
			break;
		case 2:
			UGameplayStatics::PlaySound2D(GetWorld(), dodgeSound3);
			break;
		}
	}
	else {
		GotDamage(damage);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), gunHitEffect, GetActorLocation(), FRotator::ZeroRotator, FVector(5.0f));

		if (mariaAI != nullptr) {
			if (bIsCanParryed) {
				if (AnimInstance->Montage_IsPlaying(NULL)) {
					AnimInstance->Montage_Stop(NULL);
				}
				bIsCanParryed = false;
				mariaAI->bIsStun = true;
				UGameplayStatics::PlaySound2D(GetWorld(), parryedSound);
			}
		}
		
		//패리판정 및 스턴판정이 들어감
		//UE_LOG(LogTemp, Warning, TEXT("Gun Attack Damage : %.0f"), damage);
	}
}

bool ALadyMaria::GetInStun()
{
	return mariaAI->bIsStun;
}

bool ALadyMaria::GetHoldAttackOK()
{
	return bIsHitHoldAttack;
}

float ALadyMaria::GetHealth()
{
	return healthPoint;
}

void ALadyMaria::Lockon(bool value)
{
	floatingWidgetComp->SetVisibility(value);
}

void ALadyMaria::OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABloodBornCharacter* Player = Cast<ABloodBornCharacter>(OtherActor);//들어온 액터가 플레이어인지 확인
	if (Player && bIsCanDealDamage) {//플레이어이고, 공격도 가능하다면
		//플레이어는 데미지를 입었다.
		if (mariaAI->attackState == EAttackState::QUICKSLASH) {
			UGameplayStatics::ApplyDamage(Player, weakAttack, GetInstigator()->GetController(), this, UDamageType::StaticClass());
		}
		else {
			 UGameplayStatics::ApplyDamage(Player, strongAttack, GetInstigator()->GetController(), this, UDamageType::StaticClass());
		}
		bIsCanDealDamage = false;
	}
}

void ALadyMaria::Slash()
{
	//만약 재생중이던 몽타주가 있으면
	if (AnimInstance->IsAnyMontagePlaying()) {
		//공격 취소
		return;
	}

	//스테미나 소모
	stamina -= 50.0f;

	//없으면 베기공격을 재생
	if (mariaAI->attackState == EAttackState::QUICKSLASH) {
		AnimInstance->Montage_Play(AnimQuickSlash);
	}
	else {
		AnimInstance->Montage_Play(AnimStrongSlash);
	}
}

void ALadyMaria::Thrust()
{
	//만약 재생중이던 몽타주가 있으면
	if (AnimInstance->IsAnyMontagePlaying()) {
		//공격 취소
		return;
	}

	//없으면 
	if (mariaAI->attackState == EAttackState::QUICKTHRUST) {
		//찌르기를 재생
		AnimInstance->Montage_Play(AnimQuickThrust);
	}
	else {
		AnimInstance->Montage_Play(AnimChargeThrust);
	}
	//스테미나 소모
	stamina -= 100.0f;
}

void ALadyMaria::JumpAttack()
{
	//만약 재생중이던 몽타주가 있으면
	if (AnimInstance->IsAnyMontagePlaying()) {
		//공격 취소
		return;
	}

	//없으면 점프공격을 재생
	AnimInstance->Montage_Play(AnimJumpAttack);
	//스테미나 소모
	stamina -= 100.0f;
}

void ALadyMaria::Assult()
{
	//만약 재생중이던 몽타주가 있으면
	if (AnimInstance->IsAnyMontagePlaying()) {
		//공격 취소
		return;
	}

	//없으면 돌진베기를 재생
	AnimInstance->Montage_Play(AnimAssult);
}

void ALadyMaria::AimGun()
{
	//만약 재생중이던 몽타주가 있으면
	if (AnimInstance->IsAnyMontagePlaying()) {
		//공격 취소
		return;
	}

	//없으면 사격 개시
	AnimInstance->Montage_Play(AnimFireGun);
}

void ALadyMaria::EffectOn()
{
	if (mariaAI->attackState == EAttackState::QUICKTHRUST || mariaAI->attackState == EAttackState::CHARGETHRUST) {
		rightEffect_V->SetRelativeRotation(FRotator(0.0f, -90.0f, 180.0f));
		rightEffect_H->SetRelativeRotation(FRotator(0.0f, -90.0f, -90.0f));
	}
	
	rightEffect_V->Activate(true);
	rightEffect_H->Activate(true);
	if (phaseState == EPhaseState::PHASE2) {
		if (FMath::RandRange(0, 1) == 0) {
			UGameplayStatics::PlaySound2D(GetWorld(), bloodSound1);
		}
		else {
			UGameplayStatics::PlaySound2D(GetWorld(), bloodSound2);
		}
	}
	else if (phaseState == EPhaseState::PHASE3) {
		if (FMath::RandRange(0, 1) == 0) {
			UGameplayStatics::PlaySound2D(GetWorld(), fireSound1);
		}
		else {
			UGameplayStatics::PlaySound2D(GetWorld(), fireSound2);
		}
	}
}

void ALadyMaria::MakeBloodDecal(FVector makePlace, bool bIsForPlayer)
{
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ABloodDecalActor* decal = GetWorld()->SpawnActor<ABloodDecalActor>(bloodDecal, makePlace, GetActorRotation(), params);

	if (bIsForPlayer) {
		TArray<AActor*> attachedBlood;
		playerREF->GetAttachedActors(attachedBlood);
		if (attachedBlood.Num() < 3) {
			decal->ForAttachPlayer();
			FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, true);
			decal->AttachToComponent(playerREF->GetMesh(), rules, FName("Spine2"));
		}
	}
	else {
		FVector decalLoc = decal->GetActorLocation();
		decalLoc.Z = 20.0f;
		decal->SetActorLocation(decalLoc);
	}
}

void ALadyMaria::SwingSoundPlay()
{
	int32 num = FMath::RandRange(0, 3);
	switch (num)
	{
	case 0:
		UGameplayStatics::PlaySound2D(GetWorld(), swordSwingSound1);
		break;
	case 1:
		UGameplayStatics::PlaySound2D(GetWorld(), swordSwingSound2);
		break;
	case 2:
		UGameplayStatics::PlaySound2D(GetWorld(), swordSwingSound3);
		break;
	case 3:
		UGameplayStatics::PlaySound2D(GetWorld(), swordSwingSound4);
		break;
	}
}

void ALadyMaria::ABP_FireGun()
{
	
	//탄환이 발사될 순간에 애님노티파이를 이용하여 다시 실행된다.
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//탄환을 발사한다
	ABulletActor* bullet = GetWorld()->SpawnActor<ABulletActor>(bulletFactory, bulletFirePoint->GetComponentLocation(), (playerREF->GetActorLocation() - bulletFirePoint->GetComponentLocation()).Rotation(), params);
	if (bullet != nullptr) {
		bullet->SetBulletSpeed(2000.0f);
		bullet->SetFirePower(40.0f);
	}
	UGameplayStatics::PlaySound2D(GetWorld(), gunFireSound1);
	mariaAI->attackState = EAttackState::IDLE;
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
						if (phaseState == EPhaseState::PHASE2) {
							MakeBloodDecal(FVector::ZeroVector, true);
						}
						break;
					}
				}
			}
			for (int32 i = 1; i <= 20; i++) {
				//50부터 1000까지
				FVector effectLoc = startLoc + GetActorForwardVector() * 50.0f * i;
				FRotator effectRot = GetActorForwardVector().Rotation();
				effectRot.Roll += FMath::RandRange(-180.0f, 180.0f);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), bloodThrustEffect, effectLoc, effectRot, FVector(3.0f));
				if (i % 4 == 0) {
					MakeBloodDecal(effectLoc, false);
					if (phaseState == EPhaseState::PHASE3) {
						UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), jumpAttackFireEffect, effectLoc, effectRot + FRotator(90.0f, 0.0f, 0.0f), FVector(3.0f));
					}
				}
			}
			if (phaseState == EPhaseState::PHASE2) {
				UGameplayStatics::PlaySound2D(GetWorld(), bloodExplodeSound);
			}
			else{
				UGameplayStatics::PlaySound2D(GetWorld(), fireExplodeSound);
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

void ALadyMaria::ABP_BossJumpStart()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), jumpWindStartEffect, GetActorLocation(), FRotator::ZeroRotator, FVector(1.0f));
}

void ALadyMaria::ABP_BossJumpTop()
{
	//플레이어 위치를 이용, 착지위치 계산
	//플레이어 위치에 착지 예정.
	movePlace = playerREF->GetActorLocation() + playerREF->GetActorForwardVector() * 100.0f;
	temp = 0.0f;
	jumpEffectInstance->JumpingToggle();
	EffectOn();
}

void ALadyMaria::ABP_BossJumpLand()
{
	TArray<FHitResult> hitInfos;
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(this);
	FVector attackPlace = GetActorLocation() + GetActorForwardVector() * 200.0f;
	bool bResult = GetWorld()->SweepMultiByProfile(hitInfos, attackPlace, attackPlace, FQuat::Identity, FName(TEXT("Pawn")), FCollisionShape::MakeSphere(200.0f), queryParams);
	//DrawDebugSphere(GetWorld(), attackPlace, 200.0f, 32, FColor::Red, false, 5.0f, 0, 1.0f);
	if (bResult) {
		for (auto& hit : hitInfos) {
			UE_LOG(LogTemp, Warning, TEXT("%s"), *hit.GetActor()->GetActorNameOrLabel());
			ABloodBornCharacter* player = Cast<ABloodBornCharacter>(hit.GetActor());
			if (player != nullptr) {
				UGameplayStatics::ApplyDamage(player, strongAttack, GetInstigator()->GetController(), this, UDamageType::StaticClass());
				MakeBloodDecal(FVector::ZeroVector, true);
				break;
			}
		}
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), jumpWindEndEffect, GetActorLocation(), FRotator::ZeroRotator, FVector(1.0f));

	if (phaseState == EPhaseState::PHASE2) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), jumpAttackEffect, attackPlace + FVector(0, 0, -90.0f), FRotator::ZeroRotator, FVector(1.0f));
		MakeBloodDecal(attackPlace, false);
		UGameplayStatics::PlaySound2D(GetWorld(), bloodExplodeSound);
	}
	else {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), jumpAttackFireEffect, attackPlace + FVector(0, 0, -90.0f), FRotator::ZeroRotator, FVector(5.0f));
		UGameplayStatics::PlaySound2D(GetWorld(), fireExplodeSound);
	}
}

void ALadyMaria::ABP_AssultChargeEnd()
{
	//플레이어와 2/3거리 + 좌측으로 500.0f위치로 이동
	FVector towardPlayer = playerREF->GetActorLocation() - GetActorLocation();

	movePlace = GetActorLocation() + towardPlayer * 2.0f / 3.0f + GetActorRightVector() * -900.0f;
	temp = 0.0f;

	int32 num = FMath::RandRange(0, 2);
	switch (num) {
	case 1:
		UGameplayStatics::PlaySound2D(GetWorld(), dodgeSound1);
		break;
	case 2:
		UGameplayStatics::PlaySound2D(GetWorld(), dodgeSound2);
		break;
	case 3:
		UGameplayStatics::PlaySound2D(GetWorld(), dodgeSound3);
		break;
	}
}

void ALadyMaria::ABP_AssultDodgeEnd()
{
	movePlace = playerREF->GetActorLocation() - playerREF->GetActorForwardVector() * 100.0f;
	bIsCanDealDamage = true;
	temp = 0.0f;
	assultMoving = false;
	EffectOn();
}

void ALadyMaria::ABP_Boss_Dead()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), deadBlowEffect, GetActorLocation() + GetActorForwardVector() * 200.0f, FRotator::ZeroRotator, FVector(1.0f));
	GetMesh()->SetVisibility(false, true);
	deadCloud->SetVisibility(true);
	deadCloud->Deactivate();
	FTimerHandle destoryTimer;
	GetWorldTimerManager().SetTimer(destoryTimer, FTimerDelegate::CreateLambda([&](){
		bossWidget->SetVisibility(ESlateVisibility::Hidden);
		Destroy();
		}), 4.0f, false);
}
