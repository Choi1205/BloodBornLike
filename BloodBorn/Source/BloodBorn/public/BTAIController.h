// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BTAIController.generated.h"

/**
 * 
 */
UCLASS()
class BLOODBORN_API ABTAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	void BeginPlay() override;

	ABTAIController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTree* BehaverTree;

	UPROPERTY(EditAnywhere)
	float TrackingPlayer = 2.0f;


	UPROPERTY(EditAnywhere)
	class UPawnSensingComponent* PawnSensing;

	UFUNCTION()
	void OnSeePawn(APawn* PlayerPawn);

	UFUNCTION()
	void SetCanSeePlayer(bool SeePlayer, class UObject* Player);

	FTimerHandle RetriggerableTimerHandle;
	FTimerDelegate FunctionDelegate;
	void RunRetriggerableTimer();

	UFUNCTION()
	void RandomPatrol();//랜덤이동 함수

private:
	class UNavigationSystemV1* NavArea;//길찾기 범위. 프로젝트명.Build.cs 파일도 참조할 것

	FVector RandomLocation;//다음 랜덤한 위치를 가리키는 백터
};
