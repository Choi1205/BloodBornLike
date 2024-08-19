// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BossAnimInstance.h"
#include "Enemy/LadyMaria.h"
#include "Enemy/LadyMariaAIController.h"
#include "Enemy/SmokeFXActor.h"
#include "Enemy/LadyMariaJumpEffectActor.h"
#include "BloodBornGameMode.h"

void UBossAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	mariaREF = Cast<ALadyMaria>(TryGetPawnOwner());
}

void UBossAnimInstance::NativeBeginPlay()
{
	if (mariaREF != nullptr) {
		mariaAIREF = Cast<ALadyMariaAIController>(mariaREF->GetController());
	}
	health = mariaREF->GetHealth();
}

void UBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	//���� ����� �̵��ӵ��� �̿��ؼ� ��/����, �¿��̵� üũ. ���ڸ�ǿ� ���
	if (mariaREF != nullptr) {
		FVector currentVelocity = mariaREF->GetVelocity();
		FRotator currentRot = mariaREF->GetActorRotation();
		
		FVector newVector = (currentRot.UnrotateVector(currentVelocity)).GetSafeNormal();
		moveX = newVector.X;
		moveY = newVector.Y;
		if (mariaAIREF != nullptr) {
			bIsStun = mariaAIREF->bIsStun;
		}
		bIsHitHoldAttack = mariaREF->bIsHitHoldAttack;
		health = mariaREF->GetHealth();
	}
	else {
		mariaREF = Cast<ALadyMaria>(TryGetPawnOwner());
	}
}

//���� �غ� ����
void UBossAnimInstance::AnimNotify_Boss_AttackStart()
{
	//�ִϸ��̼ǿ��� �ո��� �Ȳ����ֹǷ� Į�� ��� �������� ������.
	//�� �ִϸ��̼��� �����غ�κ��� ����ӵ��� �ʹ� �����Ƿ� ������ �����Ѵ�.
	if (mariaAIREF->attackState == EAttackState::QUICKTHRUST || mariaAIREF->attackState == EAttackState::CHARGETHRUST) {
		mariaREF->rightSword1->SetRelativeRotation(FRotator(40.0f, 0.0f, 90.0f));
		mariaREF->rightSword2->SetRelativeRotation(FRotator(40.0f, 0.0f, 90.0f));
		mariaREF->rightSword3->SetRelativeRotation(FRotator(40.0f, 0.0f, 90.0f));
		mariaREF->rightTwoSword->SetRelativeRotation(FRotator(40.0f, 0.0f, 90.0f));
		mariaREF->bIsSuperArmor = true;

		if (mariaAIREF->attackState == EAttackState::QUICKTHRUST) {
			Montage_SetPlayRate(NULL, 0.3f);
		}
		else if (mariaAIREF->attackState == EAttackState::CHARGETHRUST) {
			Montage_SetPlayRate(NULL, 0.2f);
		}
	}
	else if (mariaAIREF->attackState == EAttackState::QUICKSLASH || mariaAIREF->attackState == EAttackState::ASSULT) {
		Montage_SetPlayRate(NULL, 0.1f);
	}
	else if (mariaAIREF->attackState == EAttackState::STRONGSLASH || mariaAIREF->attackState == EAttackState::JUMPATTACK) {
		Montage_SetPlayRate(NULL, 0.5f);
	}
	
	//�ִϸ��̼��� �������� ������, ��Ʈ����� �ƴ϶� ������ ���� �����Ƿ�, ���۾Ƹ������� �ִ´�.
	if (mariaAIREF->attackState == EAttackState::JUMPATTACK) {
		mariaREF->bIsSuperArmor = true;
		//���� ����Ʈ�� �Ҵ�.
		mariaREF->jumpEffectInstance->JumpingToggle();
	}

	//���� �غ��߿��� �÷��̾ �����Ѵ�.
	mariaREF->bIsAimmingWhileAttack = true;
}

//���� �غ� ��. ���� ����.
void UBossAnimInstance::AnimNotify_Boss_SlowEnd()
{
	if (mariaAIREF->attackState == EAttackState::QUICKTHRUST) {
		Montage_SetPlayRate(mariaREF->AnimQuickThrust, 2.0f);
	}
	else if (mariaAIREF->attackState == EAttackState::CHARGETHRUST) {
		Montage_SetPlayRate(mariaREF->AnimChargeThrust, 5.0f);
	}
	else {
		Montage_SetPlayRate(NULL, 1.0f);
	}

	if (mariaREF->phaseState != EPhaseState::PHASE1) {
		//���� ����Ʈ ����
		mariaREF->EffectOn();
	}

	mariaREF->SwingSoundPlay();
	//���� ������ �Ѱ�, ���� ������ ����.
	mariaREF->bIsCanDealDamage = true;
	mariaREF->bIsAimmingWhileAttack = false;
}

//�߰Ÿ� ���, �������ݿ�. ���� ������ �߻��Ѵ�
void UBossAnimInstance::AnimNotify_Boss_2ndSlowEnd()
{
	mariaREF->ABP_2ndSlowEnd();
}

//�������� ��
void UBossAnimInstance::AnimNotify_Boss_AttackEnd()
{
	//�����ϸ鼭 �̵��� �¿� ����
	if (mariaAIREF->RandomNextMoveTF(50)) {
		mariaAIREF->bIsLeftMove = true;
	}
	else {
		mariaAIREF->bIsLeftMove = false;
	}

	//������ ���ɼ��� �� ����
	if (mariaAIREF->RandomNextMoveTF(30)) {
		mariaAIREF->bIsForwardDodge = true;
		UE_LOG(LogTemp, Warning, TEXT("ForwardDodge is True"));
	}
	else {
		mariaAIREF->bIsForwardDodge = false;
		UE_LOG(LogTemp, Warning, TEXT("ForwardDodge is False"));
	}

	mariaAIREF->attackState = EAttackState::IDLE;
	mariaREF->bIsSuperArmor = false;
}

//���� �ִϸ��̼��� ��Ʈ����� �ƴ϶� �������� �Ű�����Ѵ�.
void UBossAnimInstance::AnimNotify_Boss_MovingAttackStart()
{
	mariaREF->bIsMovingWhileAttack = true;
}

//���ݽ� ���� ��
void UBossAnimInstance::AnimNotify_Boss_MovingAttackEnd()
{
	mariaREF->bIsMovingWhileAttack = false;
	mariaREF->bIsCanDealDamage = false;
	mariaREF->assultMoving = true;
	if (mariaREF->phaseState != EPhaseState::PHASE1 && mariaAIREF->attackState == EAttackState::STRONGSLASH) {
		mariaREF->MakeBloodDecal(mariaREF->GetActorLocation(), false);
	}
}

//�÷��̾� ��ݰ��� ȸ�ǽ� ����
void UBossAnimInstance::AnimNotify_Boss_DodgeStart()
{
	mariaREF->bIsSuperArmor = true;
	if (mariaREF->smokeActor1 != nullptr) {
		mariaREF->GetMesh()->SetVisibility(false, true);
		mariaREF->smokeActor1->SetActorLocation(mariaREF->GetActorLocation());
		mariaREF->smokeActor1->PlayFX();
	}
}

//ȸ�� ��
void UBossAnimInstance::AnimNotify_Boss_DodgeEnd()
{
	mariaREF->ABP_DodgeEnd();
}

//���� �ǰ� �ִϸ��̼� ��
void UBossAnimInstance::AnimNotify_Boss_HitEnd()
{
	mariaAIREF->attackState = EAttackState::IDLE;
}

//���� źȯ�� �߻�Ǵ� ����
void UBossAnimInstance::AnimNotify_Boss_GunFire()
{
	mariaREF->ABP_FireGun();
}

//���� �и� ���� �� ����� ���� ��
void UBossAnimInstance::AnimNotify_Boss_HitHoldAttackEnd()
{
	mariaREF->bIsHitHoldAttack = true;
	mariaAIREF->bIsStun = false;
	mariaAIREF->attackState = EAttackState::IDLE;
}

//�и����� ���� �÷��� ��
void UBossAnimInstance::AnimNotify_Boss_ParryStart()
{
	mariaREF->bIsCanParryed = true;
}

//�и����� ���� �÷��� ����
void UBossAnimInstance::AnimNotify_Boss_ParryEnd()
{
	mariaREF->bIsCanParryed = false;
}

//�и� ���ϰ� �������� �ִϸ��̼� ����
void UBossAnimInstance::AnimNotify_Boss_InStunEnd()
{
	mariaAIREF->bIsStun = false;
	mariaAIREF->attackState = EAttackState::IDLE;
}

//���� �پ������ ����
void UBossAnimInstance::AnimNotify_Boss_JumpStart()
{
	//����Ʈ �ѱ�
	mariaREF->ABP_BossJumpStart();
}

//���� �ִ�ġ�϶�
void UBossAnimInstance::AnimNotify_Boss_JumpTop()
{
	//�÷��̾� ��ġ�� �̿�, ������ġ ���
	mariaREF->ABP_BossJumpTop();
	Montage_SetPlayRate(NULL, 1.0f);
}

//���� ������
void UBossAnimInstance::AnimNotify_Boss_JumpLand()
{
	//���� ����Ʈ �ѱ�, ������ ����
	mariaREF->ABP_BossJumpLand();
}

//�������� ����. �÷��̾� �������� �����δ�
void UBossAnimInstance::AnimNotify_Boss_AssultChargeEnd()
{
	Montage_SetPlayRate(NULL, 1.0f);
	mariaREF->ABP_AssultChargeEnd();
}

//�������� ���� ����. �÷��̾�� �޷����
void UBossAnimInstance::AnimNotify_Boss_AssultDodgeEnd()
{
	mariaREF->ABP_AssultDodgeEnd();
}

//���� ��� �ִϸ��̼� �� �������� ����
void UBossAnimInstance::AnimNotify_Boss_Dead()
{
	ABloodBornGameMode* gameMode = Cast<ABloodBornGameMode>(GetWorld()->GetAuthGameMode());
	gameMode->ShowClearUI();
	mariaREF->ABP_Boss_Dead();
}
