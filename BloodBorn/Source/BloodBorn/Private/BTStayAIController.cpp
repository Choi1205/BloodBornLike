// Fill out your copyright notice in the Description page of Project Settings.


#include "BTStayAIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "BloodBorn/BloodBornCharacter.h"

ABTStayAIController::ABTStayAIController() {
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
}

void ABTStayAIController::BeginPlay() {
	Super::BeginPlay();

	PawnSensing->OnSeePawn.AddDynamic(this, &ABTStayAIController::OnSeePawn);

	RunBehaviorTree(BehaverTree);

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);//현재 이 AI캐릭터가 올라서있는 Navi매쉬 범위를 가져온다.
	
}

void ABTStayAIController::OnSeePawn(APawn* PlayerPawn)
{
	ABloodBornCharacter* Player = Cast<ABloodBornCharacter>(PlayerPawn);

	if (Player) {
		SetCanSeePlayer(true, Player);
		RunRetriggerableTimer();
	}
}

void ABTStayAIController::SetCanSeePlayer(bool SeePlayer, UObject* Player)
{
	if (SeePlayer) {
		GetBlackboardComponent()->SetValueAsBool(FName("CanSeePlayer"), SeePlayer);
		GetBlackboardComponent()->SetValueAsObject(FName("PlayerTarget"), Player);
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