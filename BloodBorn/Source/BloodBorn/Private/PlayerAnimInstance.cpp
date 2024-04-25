// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "PlayerAnimInstance.h"
#include "BloodBorn/BloodBornCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AttributeComponent.h"
#include "HUD/BBPlayerHUD.h"
#include "HUD/PlayerOverlay.h"
#include "Kismet/KismetMathLibrary.h"

// ���� �ʱ�ȭ�ϰ� ���� ����
void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();  // override�� �������̱� ������ super, �θ� ver ȣ���ؼ� �θ𿡼� ���� ���� �Ͼ���� Ȯ��, �ִϸ��̼� �ν��Ͻ��� ���⼭ ����� ��

// 	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
// 	{
	PlayerCharacter = Cast<ABloodBornCharacter>(TryGetPawnOwner());  // () �ȿ� �ִ� �� <> �ȿ� �ִ� ��ü�� ����Ű�� ������ cast ���� �� null ��ȯ, () �ȿ� �����Ϸ��� �׸��� null �̸� cast �����ϰ� null ��ȯ
		if (PlayerCharacter)
		{
			PlayerCharacterMovement = PlayerCharacter->GetCharacterMovement();
		}
// 	ABBPlayerHUD* BBPlayerHUD = Cast<ABBPlayerHUD>(PlayerController->GetHUD());
// 		if (BBPlayerHUD)
// 		{
// 			PlayerOverlay = BBPlayerHUD->GetPlayerOverlay();
// 		}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTIime)
{
	Super::NativeUpdateAnimation(DeltaTIime);  //DeltaTime�� �θ� ver�� ����?

	if (PlayerCharacterMovement)  // NULL�� �ƴ� ��쿡�� �� �����Ϳ� ������
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(PlayerCharacterMovement->Velocity);  // UKismetMathLibrary Ŭ���� �ȿ� �ִ� VSizeXY ���� �Լ����� static�̹Ƿ� KismetMathLibrary ������ ��ü(object)�� ������ �ʿ䰡 ����, �� �Լ��� ���³� Ŭ������ ������ �������� ����, ���� ���� ��� �����ϰ� ��� ��ȯ�ϴ� �ϸ� ��
		Direction = UAnimInstance::CalculateDirection(PlayerCharacter->GetVelocity(), PlayerCharacter->GetActorRotation());
		CharacterState = PlayerCharacter->GetCharacterState();
		//ActionState = PlayerCharacter->GetActionState();
	}
}

void UPlayerAnimInstance::AnimNotify_Heal()
{	
	UE_LOG(LogTemp, Warning, TEXT("ANIMNOTIFY_HEAL"));
// 	if (CharacterState == ECharacterState::ECS_EquippedOneHandedWeapon)
// 	{
		//UE_LOG(LogTemp, Warning, TEXT("current state : ONEHANDED"));
	Montage_SetPlayRate(PlayerCharacter->BloodVialMontage, 0.8f);
	PlayerCharacter->Heal();
	//	UE_LOG(LogTemp, Warning, TEXT("Heal Done"));
//	}
}

void UPlayerAnimInstance::AnimNotify_Decline()
{
	UE_LOG(LogTemp, Warning, TEXT("ANIMNOTIFY_Decline"));
	Montage_SetPlayRate(PlayerCharacter->MakeBulletMontage, 0.8f);
	PlayerCharacter->Decline();
}

void UPlayerAnimInstance::AnimNotify_AttackEnd()
{
	PlayerCharacter->AttackEnd();
}

void UPlayerAnimInstance::AnimNotify_EnableBoxCollision()
{
	PlayerCharacter->SetWeaponCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UPlayerAnimInstance::AnimNotify_DisableCollision()
{
	PlayerCharacter->SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UPlayerAnimInstance::AnimNotify_HitReactEnd()
{
	PlayerCharacter->HitReactEnd();
}

void UPlayerAnimInstance::AnimNotify_EnableInvincible()
{
	PlayerCharacter->bIsInvincible =true;
}

void UPlayerAnimInstance::AnimNotify_DisableInvincible()
{
	PlayerCharacter->bIsInvincible = false;
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	PlayerCharacter->GetCharacterMovement()->MaxAcceleration = 2048.0f;
}

void UPlayerAnimInstance::AnimNotify_WeaponVisible()
{
	PlayerCharacter->WeaponVisible();
}

void UPlayerAnimInstance::AnimNotify_GunInvisible()
{
	PlayerCharacter->GunInvisible();
}

void UPlayerAnimInstance::AnimNotify_GunVisible()
{
	PlayerCharacter->GunVisible();
}

void UPlayerAnimInstance::AnimNotify_WeaponInvisible()
{
	PlayerCharacter->WeaponInvisible();
}

void UPlayerAnimInstance::AnimNotify_HoldAttackStart()
{
	PlayerCharacter->HoldAttackStart(PlayerCharacter->LockOnEnemyREF, 10.0f, EAttackType::Normal);
	Montage_SetPlayRate(PlayerCharacter->HoldAttackMontage, 0.4f);
	PlayerCharacter->bIsZoomIn = true;
}

void UPlayerAnimInstance::AnimNotify_HoldAttackFinish()
{
	PlayerCharacter->HoldAttackEnd(PlayerCharacter->LockOnEnemyREF, 500.0f, EAttackType::HoldAttack);
	Montage_SetPlayRate(PlayerCharacter->HoldAttackMontage, 1.0f);
}

void UPlayerAnimInstance::AnimNotify_RaiseArm()
{
	Montage_SetPlayRate(PlayerCharacter->FireMontage, 3.0f);
}

void UPlayerAnimInstance::AnimNotify_ReturnArm()
{
	Montage_SetPlayRate(PlayerCharacter->FireMontage, 1.0f);
}

void UPlayerAnimInstance::AnimNotify_Stay()
{
	Montage_SetPlayRate(PlayerCharacter->BloodVialMontage, 0.8f);
}

void UPlayerAnimInstance::AnimNotify_BulletStay()
{
	Montage_SetPlayRate(PlayerCharacter->MakeBulletMontage, 0.8f);
}

void UPlayerAnimInstance::AnimNotify_Back()
{
	Montage_SetPlayRate(PlayerCharacter->BloodVialMontage, 4.5f);
}

void UPlayerAnimInstance::AnimNotify_BulletBack()
{
	Montage_SetPlayRate(PlayerCharacter->MakeBulletMontage, 4.5f);
}
