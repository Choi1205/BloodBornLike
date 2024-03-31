// Fill out your copyright notice in the Description page of Project Settings.


#include "BTStayAIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "BloodBorn/BloodBornCharacter.h"

ABTStayAIController::ABTStayAIController() {
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
}

void ABTStayAIController::BeginPlay() {
	Super::BeginPlay();

	PawnSensing->OnSeePawn.AddDynamic(this, &ABTStayAIController::OnSeePawn);
	PawnSensing->OnHearNoise.AddDynamic(this, &ABTStayAIController::OnHearNoise);

	RunBehaviorTree(BehaverTree);

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);//현재 이 AI캐릭터가 올라서있는 Navi매쉬 범위를 가져온다.
	
}

void ABTStayAIController::OnSeePawn(APawn* PlayerPawn)
{
	Player = Cast<ABloodBornCharacter>(PlayerPawn);

	if (Player) {
		Cast<ACharacter>(GetPawn())->MakeNoise(1.0f, Cast<ACharacter>(GetPawn()), GetPawn()->GetActorLocation());
		SetCanSeePlayer(true, Player);
		RunRetriggerableTimer();
	}
}

void ABTStayAIController::OnHearNoise(APawn* PlayerPawn, const FVector& Location, float Volume)
{
	if (!GetBlackboardComponent()->GetValueAsBool(FName("CanSeePlayer"))) {
		ACharacter* enemyChar = Cast<ACharacter>(GetPawn());
		FRotator toward = (Location - enemyChar->GetActorLocation()).Rotation();
		enemyChar->SetActorRotation(toward);
	}
}

void ABTStayAIController::SetCanSeePlayer(bool SeePlayer, UObject* isPlayer)
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

void ABTStayAIController::RunRetriggerableTimer()
{
	if (PawnSensing->IsActive()) {
		GetWorld()->GetTimerManager().ClearTimer(RetriggerableTimerHandle);

		FunctionDelegate.BindUFunction(this, FName("SetCanSeePlayer"), false, GetPawn());

		GetWorld()->GetTimerManager().SetTimer(RetriggerableTimerHandle, FunctionDelegate, PawnSensing->SensingInterval * TrackingPlayer, false);
	}
}