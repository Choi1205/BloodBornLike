// Fill out your copyright notice in the Description page of Project Settings.


#include "LadyMariaAIController.h"
#include "NavigationSystem.h"
#include "LadyMaria.h"

ALadyMariaAIController::ALadyMariaAIController()
{
	//틱 이벤트 유효화
	PrimaryActorTick.bCanEverTick = true;
}

void ALadyMariaAIController::BeginPlay()
{
	Super::BeginPlay();

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);//현재 이 AI캐릭터가 올라서있는 Navi매쉬 범위를 가져온다.

	EnemyREF = Cast<ALadyMaria>(GetPawn());

}

void ALadyMariaAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("%f"), EnemyREF->GetPlayerDistance());

	float stamina = EnemyREF->GetBossStamina();

	//스턴상태가 아닐때 && 공격명령이 없는 상태일때
	if (!bIsStun && attackState == EAttackState::IDLE) {
		if (!bIsFireGun && EnemyREF->GetPlayerDistance() > 500.0f && !bIsDualSword) {
			bIsFireGun = true;
			EnemyREF->AimGun();
		}
		else if (EnemyREF->GetPlayerDistance() > 500.0f && EnemyREF->GetPlayerDistance() < 1000.0f && EnemyREF->phaseState != EPhaseState::PHASE1) {
			//2페이즈부터는 장거리 견제로 찌르기가 나간다.
			attackState = EAttackState::THRUST;
			EnemyREF->Thrust();
		}

		if (EnemyREF->GetPlayerDistance() >= 250.0f && EnemyREF->GetPlayerDistance() < 500.0f && bIsForwardDodge && stamina > 700.0f) {
			EnemyREF->ForwardDodge();
		}
		//플레이어와 거리가 멀거나, 스테미너가 소모된 상황일경우
		if (EnemyREF->GetPlayerDistance() >= 250.0f || stamina < 1000.0f) {
			EnemyREF->WalkToPlayer();
		}

		//1페이즈, 거리 200이하, 공격가능 스테미나, 아무 행동도 하지 않는상태, 플레이어의 속도가 100미만인 경우
		if (EnemyREF->phaseState == EPhaseState::PHASE1 && EnemyREF->GetPlayerDistance() < 200.0f && stamina > 700.0f && attackState == EAttackState::IDLE && EnemyREF->GetPlayerSpeed() < 100.0f) {
			//패링 카운터로 찌르기가 나간다.
			attackState = EAttackState::THRUST;
			EnemyREF->Thrust();
		}

		if (EnemyREF->GetPlayerDistance() < 250.0f && stamina > 700.0f && attackState == EAttackState::IDLE) {
			if (RandomNextMoveTF(50)) {
				attackState = EAttackState::RIGHTSLASH;
			}
			else {
				attackState = EAttackState::LEFTSLASH;
			}
		}
	}

	if (bIsFireGun) {
		gunFireTimer += DeltaTime;
		if (gunFireTimer > 2.0f) {
			bIsFireGun = false;
			gunFireTimer = 0.0f;
		}
	}

	if(EnemyREF->phaseState == EPhaseState::PHASE1){
		dualSwordTimer += DeltaTime;
		if(dualSwordTimer > modeChangeTime){
			if(bIsDualSword){
				bIsDualSword = false;
			}
			else{
				bIsDualSword = true;
			}
			dualSwordTimer = 0.0f;
			bIsChangeMode = true;
		}
	}
	else {
		bIsDualSword = true;
	}
}

//연속 행동을 할지 결정하는 함수
bool ALadyMariaAIController::RandomNextMoveTF(int32 rate)
{
	int32 num = FMath::RandRange(0, 99);

	if (num < rate) {
		return true;
	}
	else {
		return false;
	}
}
