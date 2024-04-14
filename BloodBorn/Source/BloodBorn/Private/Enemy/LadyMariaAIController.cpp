// Fill out your copyright notice in the Description page of Project Settings.
/*
AI컨트롤러에서는 직접 캐릭터를 작동시키지않고, 작동하는 명령만 내리도록 유의하여 코드 만들기
*/

#include "Enemy/LadyMariaAIController.h"
#include "Enemy/LadyMaria.h"

ALadyMariaAIController::ALadyMariaAIController()
{
	//틱 이벤트 유효화
	PrimaryActorTick.bCanEverTick = true;
}

void ALadyMariaAIController::BeginPlay()
{
	Super::BeginPlay();
	//시작하면 조종중인 보스를 캐싱
	EnemyREF = Cast<ALadyMaria>(GetPawn());

}

void ALadyMariaAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//보스의 스테미나량을 계속 체크
	float stamina = EnemyREF->GetBossStamina();

	//스턴상태가 아닐때 && 공격명령이 없는 상태일때, 거리가 아주 멀면 대기. 대기 상태를 만들어야 하나?
	if (!bIsStun && attackState == EAttackState::IDLE && EnemyREF->GetPlayerDistance() < veryLongRange) {
		//플레이어와 거리가 멀때 발동하는 행동부터 체크한다.
		
		//장거리 견제 명령
		//3페이즈, 초장거리 상황일때, 공격이 가능하면 눈앞으로 대시해라.
		if (EnemyREF->phaseState == EPhaseState::PHASE3 && EnemyREF->GetPlayerDistance() > longRange && stamina > moveableStamina && !bIsForwardDodge) {
			bIsForwardDodge = true;
			attackState = EAttackState::FORWARDDODGE;
			EnemyREF->ForwardDodge();
		}

		//1페이즈의 합체검 상태 && 플레이어 거리가 500 초과면
		if (EnemyREF->GetPlayerDistance() > midRange && !bIsDualSword && gunFireTimer == gunFireCooldown) {
			//총을 쏴라.
			EnemyREF->AimGun();
			gunFireTimer = 0.0f;
		}
		//2페이즈부터는 장거리 견제로 찌르기가 나간다. 아주 멀면 발동 안함
		else if (EnemyREF->GetPlayerDistance() > midRange && EnemyREF->GetPlayerDistance() < longRange && stamina > moveableStamina && EnemyREF->phaseState != EPhaseState::PHASE1 && chargeTimer == chargeCooldown) {
			//50%확율로 차지찌르기/차지베기
			if (RandomNextMoveTF(50)) {
				attackState = EAttackState::CHARGETHRUST;
				chargeTimer = 0.0f;
			}
			else {
				attackState = EAttackState::CHARGESLASH;
				chargeTimer = 0.0f;
			}
		}

		//이동 관련 명령
		//500이하, 200초과 거리, 이전 행동 종료 후 체크에서 전방대시가 ON되고, 전방대시 이후 공격이 가능할 때
		if (EnemyREF->GetPlayerDistance() > closeRange && EnemyREF->GetPlayerDistance() <= midRange && bIsForwardDodge && stamina > moveableStamina) {
			//피격리액션등, 다른 애니메이션이 재생중이 아닌경우
			if (!EnemyREF->AnimInstance->IsAnyMontagePlaying()) {
				//전방대시를 해라
				attackState = EAttackState::FORWARDDODGE;
				EnemyREF->ForwardDodge();
			}
		}
		//그 외에 200초과거리이거나, 행동 가능한 스테미나보다 적을때
		else if (EnemyREF->GetPlayerDistance() > closeRange || stamina <= moveableStamina) {
			//걸어서 접근/견제성 좌우 이동을 해라
			EnemyREF->WalkToPlayer();
		}

		//거리 200이하, 공격가능 스테미나, 플레이어의 속도가 100미만인 경우, 찌르기 쿨다운도 끝났다면
		if (EnemyREF->GetPlayerDistance() <= closeRange && stamina > moveableStamina && EnemyREF->GetPlayerSpeed() < 100.0f && chargeTimer == chargeCooldown) {
			//패링 견제로 찌르기를 해라
			attackState = EAttackState::QUICKTHRUST;
			chargeTimer = 0.0f;
		}
		//그외 상황인 경우
		else if (EnemyREF->GetPlayerDistance() <= closeRange && stamina > moveableStamina) {
			//일반공격 2종 실시
			if (RandomNextMoveTF(50)) {
				//이건 패링 가능
				attackState = EAttackState::QUICKSLASH;
			}
			else {
				//이건 패링 불가
				attackState = EAttackState::STRONGSLASH;
			}
		}
		
		//각종 타이머들

		//사격 연사를 막기위한 타이머. 없으면 제자리에서 계속 총만 쏜다.
		if (gunFireTimer < gunFireCooldown) {
			gunFireTimer = FMath::Min(gunFireTimer + DeltaTime, gunFireCooldown);
		}
		//2페이즈면 쌍검 고정
		if (EnemyREF->phaseState == EPhaseState::PHASE2 && !bIsDualSword) {
			bIsDualSword = true;
		}
		//1페이즈 무기 교체 타이머
		else if (EnemyREF->phaseState == EPhaseState::PHASE1) {
			dualSwordTimer = FMath::Min(dualSwordTimer + DeltaTime, modeChangeTime);
			if (dualSwordTimer == modeChangeTime) {
				//bIsDualSword가 true면 쌍검, flase면 합체검
				if (bIsDualSword) {
					bIsDualSword = false;
				}
				else {
					bIsDualSword = true;
				}
				dualSwordTimer = 0.0f;
				bIsChangeMode = true;
			}
		}
	}

	//찌르기/차지베기 타이머
	if (chargeTimer < chargeCooldown) {
		chargeTimer = FMath::Min(chargeTimer + DeltaTime, chargeCooldown);
	}
}

//확율이 필요할때 사용한다. rate값 확율로 true/false를 결정
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
