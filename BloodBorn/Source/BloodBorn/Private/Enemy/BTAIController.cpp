// Fill out your copyright notice in the Description page of Project Settings.
//비헤이비어 트리를 사용하는 일반몬스터의 AI

#include "Enemy/BTAIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "BloodBorn/BloodBornCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ABTAIController::ABTAIController() {
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
}

void ABTAIController::BeginPlay() {
	Super::BeginPlay();

	PawnSensing->OnSeePawn.AddDynamic(this, &ABTAIController::OnSeePawn);
	PawnSensing->OnHearNoise.AddDynamic(this, &ABTAIController::OnHearNoise);

	RunBehaviorTree(BehaverTree);

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);//현재 이 AI캐릭터가 올라서있는 Navi매쉬 범위를 가져온다.

	RandomPatrol();//랜덤이동을 바로 실시
	
}

void ABTAIController::RandomPatrol() {
	if (NavArea) {//Navi매쉬 범위 안에 있는경우
		NavArea->K2_GetRandomLocationInNavigableRadius(GetWorld(), GetPawn()->GetActorLocation(), RandomLocation, 5000.0f);//내비매쉬 범위 안에서 5000.0f범위안의 랜덤한 지점을 RandomLocation에 입력한다.

		MoveToLocation(RandomLocation);//위에서 입력된 RandomLocation으로 이동한다.
	}
	else {
		NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	}
}

//플레이어 발견시
void ABTAIController::OnSeePawn(APawn* PlayerPawn)
{
	Player = Cast<ABloodBornCharacter>(PlayerPawn);

	if (Player) {
		Cast<ACharacter>(GetPawn())->GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		Cast<ACharacter>(GetPawn())->MakeNoise(1.0f, Cast<ACharacter>(GetPawn()), GetPawn()->GetActorLocation());
		SetCanSeePlayer(true, Player);
		RunRetriggerableTimer();
	}
}

//소리를 들었을 때
void ABTAIController::OnHearNoise(APawn* PlayerPawn, const FVector& Location, float Volume)
{
	//플레이어를 발견한 상태가 아니면
	if (!GetBlackboardComponent()->GetValueAsBool(FName("CanSeePlayer"))) {
		ACharacter* enemyChar = Cast<ACharacter>(GetPawn());
		FRotator toward = (Location - enemyChar->GetActorLocation()).Rotation();
		toward.Pitch = 0;
		//플레이어 방향으로 돌아선다.
		enemyChar->SetActorRotation(toward);
	}
}

//플레이어 발견/놓침 토글
void ABTAIController::SetCanSeePlayer(bool SeePlayer, UObject* isPlayer)
{
	if (SeePlayer) {
		GetBlackboardComponent()->SetValueAsBool(FName("CanSeePlayer"), SeePlayer);
		GetBlackboardComponent()->SetValueAsObject(FName("PlayerTarget"), isPlayer);
	}
	else {
		GetBlackboardComponent()->SetValueAsBool(FName("CanSeePlayer"), SeePlayer);

		ACharacter* EnemyChar = Cast<ACharacter>(GetPawn());
		EnemyChar->GetMesh()->GetAnimInstance()->StopAllMontages(0);
	}
}

//플레이어를 놓치더라도 일정시간 플레이어를 추적한다.
void ABTAIController::RunRetriggerableTimer()
{
	if (PawnSensing->IsActive()) {
		GetWorld()->GetTimerManager().ClearTimer(RetriggerableTimerHandle);

		FunctionDelegate.BindUFunction(this, FName("SetCanSeePlayer"), false, GetPawn());

		GetWorld()->GetTimerManager().SetTimer(RetriggerableTimerHandle, FunctionDelegate, PawnSensing->SensingInterval * TrackingPlayer, false);
	}
}