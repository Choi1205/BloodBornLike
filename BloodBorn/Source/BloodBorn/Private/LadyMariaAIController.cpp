// Fill out your copyright notice in the Description page of Project Settings.


#include "LadyMariaAIController.h"
#include "../BloodBornCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"

ALadyMariaAIController::ALadyMariaAIController()
{
	
}

void ALadyMariaAIController::BeginPlay()
{
	Super::BeginPlay();

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);//현재 이 AI캐릭터가 올라서있는 Navi매쉬 범위를 가져온다.

	playerREF = FindPlayer_BP();
}

void ALadyMariaAIController::Tick(float DeltaTime)
{
	distanceToPlayer = FVector::Distance(GetPawn()->GetActorLocation(), playerREF->GetActorLocation());
}

ABloodBornCharacter* ALadyMariaAIController::FindPlayer_BP()
{
	TArray<AActor*> players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABloodBornCharacter::StaticClass(), players);

	//AShootingPlayer형태로 반환한다.
	return Cast<ABloodBornCharacter>(players[0]);
}

void ALadyMariaAIController::RandomMove()
{
	if (NavArea) {//Navi매쉬 범위 안에 있는경우
		NavArea->K2_GetRandomLocationInNavigableRadius(GetWorld(), GetPawn()->GetActorLocation(), RandomLocation, 5000.0f);//내비매쉬 범위 안에서 5000.0f범위안의 랜덤한 지점을 RandomLocation에 입력한다.

		MoveToLocation(RandomLocation);//위에서 입력된 RandomLocation으로 이동한다.
	}
}