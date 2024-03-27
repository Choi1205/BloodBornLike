// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPTreeStayEnemy.h"
#include "BloodBorn/BloodBornCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "BTStayAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/AttributeComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACPPTreeStayEnemy::ACPPTreeStayEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));//박스 콜리전 컴포넌트를 생성하여 DamageCollision이름으로 생성한다.
	DamageCollision->SetupAttachment(GetMesh(), TEXT("RightHandAttackSocket"));//생성한 박스 콜리전 컴포넌트를 매쉬에 붙인다.

	Attributes = CreateDefaultSubobject< UAttributeComponent>(TEXT("Attributes"));

}

// Called when the game starts or when spawned
void ACPPTreeStayEnemy::BeginPlay()
{
	Super::BeginPlay();

	BTStayAIController = Cast<ABTStayAIController>(GetController());//AI컨트롤러를 참조하여 사용하기 위해 변수 초기화

	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPPTreeStayEnemy::OnDealDamageOverlapBegin);

	AnimInstance = GetMesh()->GetAnimInstance();
}

// Called every frame
void ACPPTreeStayEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BTStayAIController->GetBlackboardComponent()->GetValueAsBool(FName("InStun"))) {
		stunTimer += DeltaTime;
	}

	if (stunTimer > 3.0f) {
		BTStayAIController->GetBlackboardComponent()->SetValueAsBool(FName("InStun"), false);
		stunTimer = 0.0f;
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
		}
	}
	//패리판정 및 스턴판정이 들어감
	UE_LOG(LogTemp, Warning, TEXT("Gun Attack Damage : %.0f"), damage);
}

void ACPPTreeStayEnemy::GotDamage(float damage)
{
	UE_LOG(LogTemp, Warning, TEXT("Damage : %f"), damage);
	healthPoint -= damage;
	UE_LOG(LogTemp, Warning, TEXT("Remain HP : %f"), healthPoint);
	UE_LOG(LogTemp, Warning, TEXT("Health P : %f"), Attributes->GetHealthPercent());

	if (healthPoint <= 0) {
		if (AnimInstance->Montage_IsPlaying(NULL)) {
			AnimInstance->Montage_Stop(NULL);
		}
		BTStayAIController->PawnSensing->Deactivate();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AnimInstance->Montage_Play(EnemyDyingAnimation);
		BTStayAIController->UnPossess();
		BTStayAIController = nullptr;
	}
	else {
		if (!BTStayAIController->GetBlackboardComponent()->GetValueAsBool(FName("InStun"))) {
			if (AnimInstance->Montage_IsPlaying(NULL)) {
				AnimInstance->Montage_Stop(NULL);
			}
			BTStayAIController->GetBlackboardComponent()->SetValueAsBool(FName("TakingHit"), true);
			AnimInstance->Montage_Play(EnemyHitAnimation);
		}
	}
}

void ACPPTreeStayEnemy::DyingAnimEnd()
{
	AnimInstance->Montage_Pause();
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
}

void ACPPTreeStayEnemy::HitAnimEnd()
{
	BTStayAIController->GetBlackboardComponent()->SetValueAsBool(FName("TakingHit"), false);
}
