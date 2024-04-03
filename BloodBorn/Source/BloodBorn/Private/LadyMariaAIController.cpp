// Fill out your copyright notice in the Description page of Project Settings.


#include "LadyMariaAIController.h"
#include "../BloodBornCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"

ALadyMariaAIController::ALadyMariaAIController()
{
	//틱 이벤트 유효화
	PrimaryActorTick.bCanEverTick = true;
}

void ALadyMariaAIController::BeginPlay()
{
	Super::BeginPlay();

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);//현재 이 AI캐릭터가 올라서있는 Navi매쉬 범위를 가져온다.

	playerREF = FindPlayer_BP();
	EnemyREF = Cast<ACharacter>(GetPawn());
}

void ALadyMariaAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//매 틱마다 플레이어와의 거리를 잰다
	distanceToPlayer = FVector::Distance(GetPawn()->GetActorLocation(), playerREF->GetActorLocation());
	if (distanceToPlayer > 100.0f) {
		WalkToPlayer();
	}
}

//비긴 플레이에서 실행. 플레이어 레퍼런스 획득
ABloodBornCharacter* ALadyMariaAIController::FindPlayer_BP()
{
	TArray<AActor*> players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABloodBornCharacter::StaticClass(), players);

	//AShootingPlayer형태로 반환한다.
	return Cast<ABloodBornCharacter>(players[0]);
}

void ALadyMariaAIController::WalkToPlayer()
{
	//FVector playerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	//FVector towardPlayer = playerLocation - GetActorLocation();

	//FVector randomLocation = BTAIController->GetBlackboardComponent()->GetValueAsVector(FName("RandomPatrolLocation"));
	//FVector nextMovePoint = randomLocation - GetActorLocation();
	
}
