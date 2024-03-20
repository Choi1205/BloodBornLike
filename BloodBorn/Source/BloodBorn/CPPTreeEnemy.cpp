// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPTreeEnemy.h"
#include "BloodBornCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
//#include "BTAIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

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

}

// Called when the game starts or when spawned
void ACPPTreeEnemy::BeginPlay()
{
	Super::BeginPlay();
	/*
	BTAIController = Cast<ABTAIController>(GetController());//AI컨트롤러를 참조하여 사용하기 위해 변수 초기화
	
	BTAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ACPPTreeEnemy::OnAIMoveCompleted);//컨트롤러의 행동이 종료된 것을 받아옴(?)
	
	PlayerCollisionDetection->OnComponentBeginOverlap.AddDynamic(this, &ACPPTreeEnemy::OnPlayerDetectedOverlapBegin);
	PlayerCollisionDetection->OnComponentEndOverlap.AddDynamic(this, &ACPPTreeEnemy::OnPlayerDetectedOverlapEnd);
	PlayerAttackCollisionDetection->OnComponentBeginOverlap.AddDynamic(this, &ACPPTreeEnemy::OnPlayerAttackOverlapBegin);
	PlayerAttackCollisionDetection->OnComponentEndOverlap.AddDynamic(this, &ACPPTreeEnemy::OnPlayerAttackOverlapEnd);
	*/
	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPPTreeEnemy::OnDealDamageOverlapBegin);
	
	//AnimInstance = GetMesh()->GetAnimInstance();
}

// Called every frame
void ACPPTreeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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