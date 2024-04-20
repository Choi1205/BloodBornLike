// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/CPPTreeEnemy.h"
#include "BloodBorn/BloodBornCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Enemy/BTAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/AttributeComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/WidgetComponent.h"
#include "Enemy/EnemyHPWidget.h"

// Sets default values
ACPPTreeEnemy::ACPPTreeEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	weaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	weaponMesh->SetupAttachment(GetMesh(), TEXT("RightHandAttackSocket"));

	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));//박스 콜리전 컴포넌트를 생성하여 DamageCollision이름으로 생성한다.
	DamageCollision->SetupAttachment(GetMesh(), TEXT("RightHandAttackSocket"));//생성한 박스 콜리전 컴포넌트를 매쉬에 붙인다.

	Attributes = CreateDefaultSubobject< UAttributeComponent>(TEXT("Attributes"));

	GetCharacterMovement()->MaxWalkSpeed = 100.0f;

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
void ACPPTreeEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	BTAIController = Cast<ABTAIController>(GetController());//AI컨트롤러를 참조하여 사용하기 위해 변수 초기화

	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPPTreeEnemy::OnDealDamageOverlapBegin);

	AnimInstance = GetMesh()->GetAnimInstance();

	hpWidget = Cast<UEnemyHPWidget>(floatingHPComp->GetWidget());
}

// Called every frame
void ACPPTreeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BTAIController != nullptr) {
		if (BTAIController->GetBlackboardComponent()->GetValueAsBool(FName("InStun"))) {
			stunTimer += DeltaTime;
		}

		if (stunTimer > 3.0f) {
			BTAIController->GetBlackboardComponent()->SetValueAsBool(FName("InStun"), false);
			stunTimer = 0.0f;
		}

		if (BTAIController->GetBlackboardComponent()->GetValueAsBool(FName("MovingAfterAttack")) == true) {
			AfterAttackMoving(DeltaTime);
		}
	}
}

// Called to bind functionality to input
void ACPPTreeEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACPPTreeEnemy::OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	PlayerREF = Cast<ABloodBornCharacter>(OtherActor);//들어온 액터가 플레이어인지 확인
	if (PlayerREF && CanDealDamage) {//플레이어이고, 공격도 가능하다면
		//플레이어는 데미지를 입었다.
		UGameplayStatics::ApplyDamage(PlayerREF, Damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
	}
}

void ACPPTreeEnemy::GetHit(const FVector& ImpactPoint)
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
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);  //Forward 노멀라이즈해서 길이 1이라 60 곱함
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
}

float ACPPTreeEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

void ACPPTreeEnemy::GotParryAttackCPP(float damage)
{
	GotDamage(damage);

	if (BTAIController != nullptr) {
		if (CanParryed) {
			if (AnimInstance->Montage_IsPlaying(NULL)) {
				AnimInstance->Montage_Stop(NULL);
			}
			CanParryed = false;
			BTAIController->GetBlackboardComponent()->SetValueAsBool(FName("InStun"), true);
			BTAIController->GetBlackboardComponent()->SetValueAsBool(FName("TakingHit"), false);
		}
	}
	//패리판정 및 스턴판정이 들어감
	UE_LOG(LogTemp, Warning, TEXT("Gun Attack Damage : %.0f"), damage);
}

bool ACPPTreeEnemy::GetInStun()
{
	return BTAIController->GetBlackboardComponent()->GetValueAsBool(FName("InStun"));
}

float ACPPTreeEnemy::GetHealth()
{
	return healthPoint;
}

void ACPPTreeEnemy::Lockon(bool value)
{
	bIsLockedOn = value;
	floatingLightComp->SetVisibility(value);
	floatingHPComp->SetVisibility(value);
}

void ACPPTreeEnemy::AfterAttackMoving(float DeltaTime)
{
	if (healthPoint > 0) {
		FVector playerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
		FVector towardPlayer = playerLocation - GetActorLocation();
		FRotator towardRot = towardPlayer.Rotation();
		towardRot.Pitch = 0;

		FVector randomLocation = BTAIController->GetBlackboardComponent()->GetValueAsVector(FName("RandomPatrolLocation"));
		FVector nextMovePoint = randomLocation - GetActorLocation();

		float randomTime = BTAIController->GetBlackboardComponent()->GetValueAsFloat(FName("RandomTime"));

		UE_LOG(LogTemp, Warning, TEXT("%f"), GetCharacterMovement()->MaxWalkSpeed);
		AddMovementInput(nextMovePoint.GetSafeNormal(1.0));
		SetActorRotation(towardRot);

		randomTime -= DeltaTime;

		BTAIController->GetBlackboardComponent()->SetValueAsFloat(FName("RandomTime"), randomTime);
	}
}

void ACPPTreeEnemy::GotDamage(float damage)
{
	healthPoint = FMath::Max(healthPoint - damage, 0.0f);

	CanDealDamage = false;

	//출혈 이펙트 재생부. 블루프린트의 NiaSys에 미리 파티클을 등록해야한다.
	bleeding = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiaSys, GetActorLocation(), FRotator::ZeroRotator);

	if (BTAIController != nullptr) {
		if (healthPoint == 0) {
			if (AnimInstance->Montage_IsPlaying(NULL)) {
				AnimInstance->Montage_Stop(NULL);
			}
			BTAIController->PawnSensing->Deactivate();
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			AnimInstance->Montage_Play(EnemyDyingAnimation);
			BTAIController->UnPossess();
			BTAIController = nullptr;
			Lockon(false);
		}
		else {
			if (!BTAIController->GetBlackboardComponent()->GetValueAsBool(FName("InStun"))) {
				if (AnimInstance->Montage_IsPlaying(NULL)) {
					AnimInstance->Montage_Stop(NULL);
				}
				BTAIController->GetBlackboardComponent()->SetValueAsBool(FName("TakingHit"), true);
				AnimInstance->Montage_Play(EnemyHitAnimation);
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

void ACPPTreeEnemy::DyingAnimEnd()
{
	AnimInstance->Montage_Pause();
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	//GetMesh()->SetSimulatePhysics(true);
}

void ACPPTreeEnemy::HitAnimEnd()
{
	if (healthPoint > 0) {
		BTAIController->GetBlackboardComponent()->SetValueAsBool(FName("TakingHit"), false);
	}
}
