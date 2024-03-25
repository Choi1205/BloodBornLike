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

	RunBehaviorTree(BehaverTree);

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);//���� �� AIĳ���Ͱ� �ö��ִ� Navi�Ž� ������ �����´�.

	RandomPatrol();//�����̵��� �ٷ� �ǽ�
	
}

void ABTAIController::RandomPatrol() {
	if (NavArea) {//Navi�Ž� ���� �ȿ� �ִ°��
		NavArea->K2_GetRandomLocationInNavigableRadius(GetWorld(), GetPawn()->GetActorLocation(), RandomLocation, 5000.0f);//����Ž� ���� �ȿ��� 5000.0f�������� ������ ������ RandomLocation�� �Է��Ѵ�.

		MoveToLocation(RandomLocation);//������ �Էµ� RandomLocation���� �̵��Ѵ�.
	}
}

void ABTAIController::OnSeePawn(APawn* PlayerPawn)
{
	ABloodBornCharacter* Player = Cast<ABloodBornCharacter>(PlayerPawn);

	if (Player) {
		SetCanSeePlayer(true, Player);
		RunRetriggerableTimer();
	}
}

void ABTAIController::SetCanSeePlayer(bool SeePlayer, UObject* Player)
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

void ABTAIController::RunRetriggerableTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(RetriggerableTimerHandle);

	FunctionDelegate.BindUFunction(this, FName("SetCanSeePlayer"), false, GetPawn());

	GetWorld()->GetTimerManager().SetTimer(RetriggerableTimerHandle, FunctionDelegate, PawnSensing->SensingInterval * TrackingPlayer, false);
}
