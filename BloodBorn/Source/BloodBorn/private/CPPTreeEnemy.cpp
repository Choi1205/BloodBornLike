// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPTreeEnemy.h"
#include "BloodBorn/BloodBornCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "BTAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/AttributeComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"

/*
플레이어를 인식한 상태에서 죽으면 발생하는 에러.
죽기전에 폰 센싱을 꺼주고, 폰 센싱 부분에 켜져있는지 체크하는 if문을 넣으면 안전하다?
Unhandled Exception: EXCEPTION_ACCESS_VIOLATION reading address 0x00000000000004a8

UnrealEditor_BloodBorn_0046!UE::CoreUObject::Private::ResolveObjectHandle() [C:\Program Files\Epic Games\UE_5.3\Engine\Source\Runtime\CoreUObject\Public\UObject\ObjectHandle.h:217]
UnrealEditor_BloodBorn_0046!ABTAIController::SetCanSeePlayer() [C:\Users\Choi\Documents\Unreal Projects\BloodBorn\Source\BloodBorn\Private\BTAIController.cpp:59]
UnrealEditor_BloodBorn_0046!ABTAIController::execSetCanSeePlayer() [C:\Users\Choi\Documents\Unreal Projects\BloodBorn\Intermediate\Build\Win64\UnrealEditor\Inc\BloodBorn\UHT\BTAIController.gen.cpp:36]
UnrealEditor_CoreUObject
UnrealEditor_CoreUObject
UnrealEditor_Engine
UnrealEditor_BloodBorn_0046!TBaseUFunctionDelegateInstance<ABTAIController,void __cdecl(void),FNotThreadSafeNotCheckedDelegateUserPolicy,bool,APawn *>::Execute() [C:\Program Files\Epic Games\UE_5.3\Engine\Source\Runtime\Core\Public\Delegates\DelegateInstancesImpl.h:154]
*/

// Sets default values
ACPPTreeEnemy::ACPPTreeEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	/*
	PlayerCollisionDetection =
		CreateDefaultSubobject<USphereComponent>(TEXT("Player Collision Detection"));//스피어 콜리전 컴포넌트를 생성하여 PlayerCollisionDetection이름으로 생성한다
	PlayerCollisionDetection->SetupAttachment(RootComponent);//생성한 스피어 콜리전 컴포넌트를 루트에 붙인다.
	PlayerAttackCollisionDetection =
		CreateDefaultSubobject<USphereComponent>(TEXT("Player Attack Collision Detection"));//스피어 콜리전 컴포넌트를 생성하여 PlayerAttackCollisionDetection이름으로 생성한다
	PlayerAttackCollisionDetection->SetupAttachment(RootComponent);//생성한 스피어 콜리전 컴포넌트를 루트에 붙인다.
	*/
	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));//박스 콜리전 컴포넌트를 생성하여 DamageCollision이름으로 생성한다.
	DamageCollision->SetupAttachment(GetMesh(), TEXT("RightHandAttackSocket"));//생성한 박스 콜리전 컴포넌트를 매쉬에 붙인다.

	Attributes = CreateDefaultSubobject< UAttributeComponent>(TEXT("Attributes"));

}

// Called when the game starts or when spawned
void ACPPTreeEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	BTAIController = Cast<ABTAIController>(GetController());//AI컨트롤러를 참조하여 사용하기 위해 변수 초기화
	/*
	BTAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ACPPTreeEnemy::OnAIMoveCompleted);//컨트롤러의 행동이 종료된 것을 받아옴(?)
	
	PlayerCollisionDetection->OnComponentBeginOverlap.AddDynamic(this, &ACPPTreeEnemy::OnPlayerDetectedOverlapBegin);
	PlayerCollisionDetection->OnComponentEndOverlap.AddDynamic(this, &ACPPTreeEnemy::OnPlayerDetectedOverlapEnd);
	PlayerAttackCollisionDetection->OnComponentBeginOverlap.AddDynamic(this, &ACPPTreeEnemy::OnPlayerAttackOverlapBegin);
	PlayerAttackCollisionDetection->OnComponentEndOverlap.AddDynamic(this, &ACPPTreeEnemy::OnPlayerAttackOverlapEnd);
	*/
	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPPTreeEnemy::OnDealDamageOverlapBegin);
	
	AnimInstance = GetMesh()->GetAnimInstance();
}

// Called every frame
void ACPPTreeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BTAIController->GetBlackboardComponent()->GetValueAsBool(FName("InStun"))) {
		stunTimer += DeltaTime;
	}

	if (stunTimer > 3.0f) {
		BTAIController->GetBlackboardComponent()->SetValueAsBool(FName("InStun"), false);
		stunTimer = 0.0f;
	}
}

// Called to bind functionality to input
void ACPPTreeEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

/*
void ACPPTreeEnemy::OnAIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) {//이동이 종료되었을때
	if (!PlayerDetected) {//플레이어가 감지범위에 없으면
		BTAIController->RandomPatrol();//랜덤탐색을 한다
	}
	else if (PlayerDetected && CanAttackPlayer) {//플레이어가 감지범위에 있고, 공격범위 안에도 있으면
		StopSeekingPlayer();//추적을 중지하고

		AnimInstance->Montage_Play(EnemyAttackAnimation);//플레이어에게 공격 행동을 한다
	}
}

void ACPPTreeEnemy::MoveToPlayer() {//플레이어에게 이동
	BTAIController->MoveToLocation(PlayerREF->GetActorLocation(), StoppingDistance, true);//랜덤이동 지점에 플레이어 지점을 입력해서 랜덤이동이 플레이어 이동이 되게 한다.
}

void ACPPTreeEnemy::SeekPlayer() {//플레이어의 위치를 갱신
	MoveToPlayer();//플레이어에게 이동한다
	GetWorld()->GetTimerManager().SetTimer(SeekPlayerTimerHandle, this, &ACPPTreeEnemy::SeekPlayer, 0.25f, true);//플레이어의 위치를 0.25초마다 갱신한다.
}

void ACPPTreeEnemy::StopSeekingPlayer() {//플레이어 추적 중단
	GetWorld()->GetTimerManager().ClearTimer(SeekPlayerTimerHandle);//플레이어 위치갱신 타이머를 리셋해서 추적을 멈춘다.
}

void ACPPTreeEnemy::OnPlayerDetectedOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	//플레이어가 탐지범위에 들어온 경우
	PlayerREF = Cast<ABloodBornCharacter>(OtherActor);//들어온 액터가 플레이어인지 확인
	if (PlayerREF) {//플레이어면
		PlayerDetected = true;//플레이어 탐지를 true로
		SeekPlayer();//추적 시작
	}
}

void ACPPTreeEnemy::OnPlayerDetectedOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	//플레이어가 탐지범위를 빠져나간 경우
	PlayerREF = Cast<ABloodBornCharacter>(OtherActor);//빠져나간 액터가 플레이어인지 확인
	if (PlayerREF) {//플레이어면
		PlayerDetected = false;//플레이어 탐지를 false로
		StopSeekingPlayer();//추적 중지
		BTAIController->RandomPatrol();//랜덤 탐색을 실행
	}
}

void ACPPTreeEnemy::OnPlayerAttackOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	//플레이어가 공격가능 범위에 들어온 경우
	PlayerREF = Cast<ABloodBornCharacter>(OtherActor);//들어온 액터가 플레이어인지 확인
	if (PlayerREF) {//플레이어면
		CanAttackPlayer = true;//공격 가능 변수를 true로
	}
}

void ACPPTreeEnemy::OnPlayerAttackOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	PlayerREF = Cast<ABloodBornCharacter>(OtherActor);//빠져나간 액터가 플레이어인지 확인
	if (PlayerREF) {//플레이어면
		CanAttackPlayer = false;//공격 가능 변수를 false로

		AnimInstance->Montage_Stop(0.0f, EnemyAttackAnimation);
		//재생중이던 공격 애니메이션을 즉시(0.0f) 정지함
		SeekPlayer();//그리고 추적을 시작함
	}
}
*/
void ACPPTreeEnemy::OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	PlayerREF = Cast<ABloodBornCharacter>(OtherActor);//들어온 액터가 플레이어인지 확인
	if (PlayerREF && CanDealDamage) {//플레이어이고, 공격도 가능하다면
		UE_LOG(LogTemp, Warning, TEXT("Player Damaged"));//플레이어는 데미지를 입었다.
	}
}
/*
void ACPPTreeEnemy::AttackAnimationEnded()
{
	if (CanAttackPlayer) {//플레이어가 아직 공격 범위에 있을 경우
		AnimInstance->Montage_Play(EnemyAttackAnimation);
		//공격 애니메이션을 다시 재생한다.
	}
}*/

void ACPPTreeEnemy::GotDamagedCPP(float Damage)
{
	//플레이어 HP UI 대응용 -HP방지
	healthPoint -= Damage;
	if (healthPoint < 0) {
		healthPoint = 0;
	}

	//데미지를 받아 경직이 들어가야한다.
	//경직치 처리는 보류
	UE_LOG(LogTemp, Warning, TEXT("Remain HP : %.0f"), healthPoint);

	if (healthPoint == 0) {
		//사망처리부분. 애니메이션 재생 필요
		//Destroy();
	}
}

void ACPPTreeEnemy::GotParryAttackCPP(float Damage)
{
	//일단 데미지가 들어감...서순은 천천히 생각하기
	GotDamagedCPP(Damage);
	
	if (CanParryed) {
		if (AnimInstance->Montage_IsPlaying(NULL)) {
			AnimInstance->Montage_Stop(NULL);
		}
		CanParryed = false;
		BTAIController->GetBlackboardComponent()->SetValueAsBool(FName("InStun"), true);
		
	}
	//패리판정 및 스턴판정이 들어감
	UE_LOG(LogTemp, Warning, TEXT("Gun Attack Damage : %.0f"), Damage);
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

	UE_LOG(LogTemp, Warning, TEXT("Damage : %f"), DamageAmount);
	healthPoint -= DamageAmount;
	UE_LOG(LogTemp, Warning, TEXT("Remain HP : %f"), healthPoint);
	UE_LOG(LogTemp, Warning, TEXT("Health P : %f"), Attributes->GetHealthPercent());

	if (healthPoint <= 0 && !bIsDead) {
		if (AnimInstance->Montage_IsPlaying(NULL)) {
			AnimInstance->Montage_Stop(NULL);
			bIsDead = true;
		}
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AnimInstance->Montage_Play(EnemyDyingAnimation);
		BTAIController->UnPossess();
	}
	
	return DamageAmount;
}

void ACPPTreeEnemy::DyingAnimEnd()
{
	AnimInstance->Montage_Pause();
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
}