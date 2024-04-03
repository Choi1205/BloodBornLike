// Fill out your copyright notice in the Description page of Project Settings.


#include "LadyMariaAIController.h"
#include "../BloodBornCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"

ALadyMariaAIController::ALadyMariaAIController()
{
	//ƽ �̺�Ʈ ��ȿȭ
	PrimaryActorTick.bCanEverTick = true;
}

void ALadyMariaAIController::BeginPlay()
{
	Super::BeginPlay();

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);//���� �� AIĳ���Ͱ� �ö��ִ� Navi�Ž� ������ �����´�.

	playerREF = FindPlayer_BP();
	EnemyREF = Cast<ACharacter>(GetPawn());
}

void ALadyMariaAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//�� ƽ���� �÷��̾���� �Ÿ��� ���
	distanceToPlayer = FVector::Distance(GetPawn()->GetActorLocation(), playerREF->GetActorLocation());
	if (distanceToPlayer > 100.0f) {
		WalkToPlayer();
	}
}

//��� �÷��̿��� ����. �÷��̾� ���۷��� ȹ��
ABloodBornCharacter* ALadyMariaAIController::FindPlayer_BP()
{
	TArray<AActor*> players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABloodBornCharacter::StaticClass(), players);

	//AShootingPlayer���·� ��ȯ�Ѵ�.
	return Cast<ABloodBornCharacter>(players[0]);
}

void ALadyMariaAIController::WalkToPlayer()
{
	//FVector playerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	//FVector towardPlayer = playerLocation - GetActorLocation();

	//FVector randomLocation = BTAIController->GetBlackboardComponent()->GetValueAsVector(FName("RandomPatrolLocation"));
	//FVector nextMovePoint = randomLocation - GetActorLocation();
	
}
