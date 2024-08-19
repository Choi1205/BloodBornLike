// Fill out your copyright notice in the Description page of Project Settings.
//�����̺�� Ʈ���� ����ϴ� �Ϲݸ����� AI

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

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);//���� �� AIĳ���Ͱ� �ö��ִ� Navi�Ž� ������ �����´�.

	RandomPatrol();//�����̵��� �ٷ� �ǽ�
	
}

void ABTAIController::RandomPatrol() {
	if (NavArea) {//Navi�Ž� ���� �ȿ� �ִ°��
		NavArea->K2_GetRandomLocationInNavigableRadius(GetWorld(), GetPawn()->GetActorLocation(), RandomLocation, 5000.0f);//����Ž� ���� �ȿ��� 5000.0f�������� ������ ������ RandomLocation�� �Է��Ѵ�.

		MoveToLocation(RandomLocation);//������ �Էµ� RandomLocation���� �̵��Ѵ�.
	}
	else {
		NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	}
}

//�÷��̾� �߽߰�
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

//�Ҹ��� ����� ��
void ABTAIController::OnHearNoise(APawn* PlayerPawn, const FVector& Location, float Volume)
{
	//�÷��̾ �߰��� ���°� �ƴϸ�
	if (!GetBlackboardComponent()->GetValueAsBool(FName("CanSeePlayer"))) {
		ACharacter* enemyChar = Cast<ACharacter>(GetPawn());
		FRotator toward = (Location - enemyChar->GetActorLocation()).Rotation();
		toward.Pitch = 0;
		//�÷��̾� �������� ���Ƽ���.
		enemyChar->SetActorRotation(toward);
	}
}

//�÷��̾� �߰�/��ħ ���
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

//�÷��̾ ��ġ���� �����ð� �÷��̾ �����Ѵ�.
void ABTAIController::RunRetriggerableTimer()
{
	if (PawnSensing->IsActive()) {
		GetWorld()->GetTimerManager().ClearTimer(RetriggerableTimerHandle);

		FunctionDelegate.BindUFunction(this, FName("SetCanSeePlayer"), false, GetPawn());

		GetWorld()->GetTimerManager().SetTimer(RetriggerableTimerHandle, FunctionDelegate, PawnSensing->SensingInterval * TrackingPlayer, false);
	}
}