// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BTStayAIController.generated.h"

/**
 * 
 */
UCLASS()
class BLOODBORN_API ABTStayAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	void BeginPlay() override;

	ABTStayAIController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTree* BehaverTree;

	UPROPERTY(EditAnywhere)
	float TrackingPlayer = 25.0f;


	UPROPERTY(EditAnywhere)
	class UPawnSensingComponent* PawnSensing;

	UFUNCTION()
	void OnSeePawn(APawn* PlayerPawn);

	UFUNCTION()
	void OnHearNoise(APawn* PlayerPawn, const FVector &Location, float Volume);

	UFUNCTION()
	void SetCanSeePlayer(bool SeePlayer, class UObject* Player);

	FTimerHandle RetriggerableTimerHandle;
	FTimerDelegate FunctionDelegate;
	void RunRetriggerableTimer();


private:
	class UNavigationSystemV1* NavArea;//��ã�� ����. ������Ʈ��.Build.cs ���ϵ� ������ ��

	FVector RandomLocation;//���� ������ ��ġ�� ����Ű�� ����

	ABloodBornCharacter* Player;
};
