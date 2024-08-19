// Fill out your copyright notice in the Description page of Project Settings.
//비헤이비어 트리를 CPP를 이용해 만든 에너미에서 순찰기능을 뺀 에너미. 매복용으로 사용


#include "Enemy/CPPTreeStayEnemy.h"
#include "BloodBorn/BloodBornCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/AttributeComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Enemy/BTStayAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Enemy/EnemyHPWidget.h"

// Sets default values
ACPPTreeStayEnemy::ACPPTreeStayEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	weaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	weaponMesh->SetupAttachment(GetMesh(), TEXT("RightHandAttackSocket"));

	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));//박스 콜리전 컴포넌트를 생성하여 DamageCollision이름으로 생성한다.
	DamageCollision->SetupAttachment(GetMesh(), TEXT("RightHandAttackSocket"));//생성한 박스 콜리전 컴포넌트를 매쉬에 붙인다.

	Attributes = CreateDefaultSubobject< UAttributeComponent>(TEXT("Attributes"));

	GetCharacterMovement()->MaxWalkSpeed = 500.0f;

	floatingLightComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("floatingWidgetComp"));
	floatingLightComp->SetupAttachment(GetMesh(), FName("spine_01"));
	floatingLightComp->SetWidgetSpace(EWidgetSpace::Screen);
	floatingLightComp->SetVisibility(false);

	floatingHPComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("floatingHPComp"));
	floatingHPComp->SetupAttachment(GetMesh(), FName("headSocket"));
	floatingHPComp->SetWidgetSpace(EWidgetSpace::Screen);
	floatingHPComp->SetVisibility(false);
}

// Called when the game starts or when spawned
void ACPPTreeStayEnemy::BeginPlay()
{
	Super::BeginPlay();

	BTStayAIController = Cast<ABTStayAIController>(GetController());//AI컨트롤러를 참조하여 사용하기 위해 변수 초기화

	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPPTreeStayEnemy::OnDealDamageOverlapBegin);

	AnimInstance = GetMesh()->GetAnimInstance();

	hpWidget = Cast<UEnemyHPWidget>(floatingHPComp->GetWidget());
}

// Called every frame
void ACPPTreeStayEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BTStayAIController != nullptr) {
		if (BTStayAIController->GetBlackboardComponent()->GetValueAsBool(FName("InStun"))) {
			stunTimer += DeltaTime;
		}

		if (stunTimer > 3.0f) {
			BTStayAIController->GetBlackboardComponent()->SetValueAsBool(FName("InStun"), false);
			stunTimer = 0.0f;
		}

		if (BTStayAIController->GetBlackboardComponent()->GetValueAsBool(FName("MovingAfterAttack")) == true) {
			AfterAttackMoving(DeltaTime);
		}
	}
}

// Called to bind functionality to input
void ACPPTreeStayEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACPPTreeStayEnemy::OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	PlayerREF = Cast<ABloodBornCharacter>(OtherActor);//들어온 액터가 플레이어인지 확인
	if (PlayerREF && CanDealDamage) {//플레이어이고, 공격도 가능하다면
		//플레이어는 데미지를 입었다.
		UGameplayStatics::ApplyDamage(PlayerREF, Damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
	}
}

void ACPPTreeStayEnemy::GetHit(const FVector& ImpactPoint)
{
	UE_LOG(LogTemp, Warning, TEXT("ImpactPoint"));

	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation().GetSafeNormal());

	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	}
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);  //Forward 노멀라이즈해서 길이 1이라 60 곱함
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
}

float ACPPTreeStayEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

void ACPPTreeStayEnemy::GotParryAttackCPP(float damage)
{
	GotDamage(damage);

	if (BTStayAIController != nullptr) {
		if (CanParryed) {
			if (AnimInstance->Montage_IsPlaying(NULL)) {
				AnimInstance->Montage_Stop(NULL);
			}
			CanParryed = false;
			BTStayAIController->GetBlackboardComponent()->SetValueAsBool(FName("InStun"), true);
			BTStayAIController->GetBlackboardComponent()->SetValueAsBool(FName("TakingHit"), false);
			UGameplayStatics::PlaySound2D(GetWorld(), parryedSound);
		}
	}
	//패리판정 및 스턴판정이 들어감
	//UE_LOG(LogTemp, Warning, TEXT("Gun Attack Damage : %.0f"), damage);
}

bool ACPPTreeStayEnemy::GetInStun()
{
	return BTStayAIController->GetBlackboardComponent()->GetValueAsBool(FName("InStun"));
}

bool ACPPTreeStayEnemy::GetHoldAttackOK()
{
	return true;
}

float ACPPTreeStayEnemy::GetHealth()
{
	return healthPoint;
}

void ACPPTreeStayEnemy::Lockon(bool value)
{
	bIsLockedOn = value;
	floatingLightComp->SetVisibility(value);
	floatingHPComp->SetVisibility(value);
}

void ACPPTreeStayEnemy::AfterAttackMoving(float DeltaTime)
{
	if (healthPoint > 0) {
		FVector playerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
		FVector towardPlayer = playerLocation - GetActorLocation();
		FRotator towardRot = towardPlayer.Rotation();
		towardRot.Pitch = 0;

		FVector randomLocation = BTStayAIController->GetBlackboardComponent()->GetValueAsVector(FName("RandomPatrolLocation"));
		FVector nextMovePoint = randomLocation - GetActorLocation();

		float randomTime = BTStayAIController->GetBlackboardComponent()->GetValueAsFloat(FName("RandomTime"));

		UE_LOG(LogTemp, Warning, TEXT("%f"), GetCharacterMovement()->MaxWalkSpeed);
		AddMovementInput(nextMovePoint.GetSafeNormal(1.0));
		SetActorRotation(towardRot);

		randomTime -= DeltaTime;

		BTStayAIController->GetBlackboardComponent()->SetValueAsFloat(FName("RandomTime"), randomTime);
	}
}

void ACPPTreeStayEnemy::GotDamage(float damage)
{
	healthPoint = FMath::Max(healthPoint - damage, 0.0f);

	CanDealDamage = false;

	//출혈 이펙트 재생부. 블루프린트의 NiaSys에 미리 파티클을 등록해야한다.
	bleeding = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiaSys, GetActorLocation(), FRotator::ZeroRotator);

	if (BTStayAIController != nullptr) {
		if (healthPoint == 0) {
			if (AnimInstance->Montage_IsPlaying(NULL)) {
				AnimInstance->Montage_Stop(NULL);
			}
			BTStayAIController->PawnSensing->Deactivate();
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			AnimInstance->Montage_Play(EnemyDyingAnimation);
			BTStayAIController->UnPossess();
			BTStayAIController = nullptr;
			Lockon(false);

			if (damage >= 500) {
				UGameplayStatics::PlaySound2D(GetWorld(), hitHoldAttackSound);
			}
			if (FMath::RandRange(0, 1) == 0) {
				UGameplayStatics::PlaySound2D(GetWorld(), dieSound1);
			}
			else {
				UGameplayStatics::PlaySound2D(GetWorld(), dieSound2);
			}
		}
		else {
			if (!BTStayAIController->GetBlackboardComponent()->GetValueAsBool(FName("InStun"))) {
				if (AnimInstance->Montage_IsPlaying(NULL)) {
					AnimInstance->Montage_Stop(NULL);
				}
				BTStayAIController->GetBlackboardComponent()->SetValueAsBool(FName("TakingHit"), true);
				AnimInstance->Montage_Play(EnemyHitAnimation);
			}

			if (damage >= 500) {
				UGameplayStatics::PlaySound2D(GetWorld(), hitHoldAttackSound);
			}
			else {
				if (FMath::RandRange(0, 1) == 0) {
					UGameplayStatics::PlaySound2D(GetWorld(), hitSound1);
				}
				else {
					UGameplayStatics::PlaySound2D(GetWorld(), hitSound2);
				}
			}

			hpWidget->SetHealthBar(healthPoint / maxHealth, damage);

			if (!bIsLockedOn) {
				floatingHPComp->SetVisibility(true);

				if (GetWorld()->GetTimerManager().IsTimerActive(showingTimer)) {
					GetWorld()->GetTimerManager().ClearTimer(showingTimer);
				}
				GetWorld()->GetTimerManager().SetTimer(showingTimer, FTimerDelegate::CreateLambda([&]() {
					floatingHPComp->SetVisibility(false);
					}), 5.0f, false);
			}
		}
	}
}

void ACPPTreeStayEnemy::DyingAnimEnd()
{
	AnimInstance->Montage_Pause();
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	//GetMesh()->SetSimulatePhysics(true);
}

void ACPPTreeStayEnemy::HitAnimEnd()
{
	if (healthPoint > 0) {
		BTStayAIController->GetBlackboardComponent()->SetValueAsBool(FName("TakingHit"), false);
	}
}
