// Fill out your copyright notice in the Description page of Project Settings.


#include "BTAIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "BloodBorn/BloodBornCharacter.h"

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
}

void ABTAIController::OnSeePawn(APawn* PlayerPawn)
{
	Player = Cast<ABloodBornCharacter>(PlayerPawn);

	if (Player) {
		Cast<ACharacter>(GetPawn())->MakeNoise(1.0f, Cast<ACharacter>(GetPawn()), GetPawn()->GetActorLocation());
		SetCanSeePlayer(true, Player);
		RunRetriggerableTimer();
	}
}

void ABTAIController::OnHearNoise(APawn* PlayerPawn, const FVector& Location, float Volume)
{
	ABloodBornCharacter* tempPlayer = Cast<ABloodBornCharacter>(PlayerPawn);

	if (tempPlayer != nullptr) {
		ACharacter* enemyChar = Cast<ACharacter>(GetPawn());
		FRotator toward = (Location - enemyChar->GetActorLocation()).Rotation();
		enemyChar->SetActorRotation(toward);
	}
	else {
		if (!noiseCheaker) {
			noiseCheaker = true;
			ACharacter* enemyChar = Cast<ACharacter>(GetPawn());
			FRotator toward = (Location - enemyChar->GetActorLocation()).Rotation();
			enemyChar->SetActorRotation(toward);
		}
	}
}

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

void ABTAIController::RunRetriggerableTimer()
{
	if (PawnSensing->IsActive()) {
		GetWorld()->GetTimerManager().ClearTimer(RetriggerableTimerHandle);

		FunctionDelegate.BindUFunction(this, FName("SetCanSeePlayer"), false, GetPawn());

		GetWorld()->GetTimerManager().SetTimer(RetriggerableTimerHandle, FunctionDelegate, PawnSensing->SensingInterval * TrackingPlayer, false);
	}
}