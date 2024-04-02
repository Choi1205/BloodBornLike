// Fill out your copyright notice in the Description page of Project Settings.


#include "LadyMariaAIController.h"
#include "../BloodBornCharacter.h"
#include "Kismet/GameplayStatics.h"

ALadyMariaAIController::ALadyMariaAIController()
{
	
}

void ALadyMariaAIController::BeginPlay()
{
	Super::BeginPlay();

	playerREF = FindPlayer_BP();
}

ABloodBornCharacter* ALadyMariaAIController::FindPlayer_BP()
{
	TArray<AActor*> players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABloodBornCharacter::StaticClass(), players);

	//AShootingPlayer���·� ��ȯ�Ѵ�.
	return Cast<ABloodBornCharacter>(players[0]);
}
