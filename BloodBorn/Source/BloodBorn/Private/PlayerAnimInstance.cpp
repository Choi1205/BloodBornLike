// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "PlayerAnimInstance.h"
#include "BloodBorn/BloodBornCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AttributeComponent.h"
#include "HUD/BBPlayerHUD.h"
#include "HUD/PlayerOverlay.h"
#include "Kismet/KismetMathLibrary.h"

// 변수 초기화하고 변수 생성
void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();  // override인 재정의이기 때문에 super, 부모 ver 호출해서 부모에서 무슨 일이 일어나는지 확인, 애니메이션 인스턴스도 여기서 실행될 것

// 	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
// 	{
	PlayerCharacter = Cast<ABloodBornCharacter>(TryGetPawnOwner());  // () 안에 있는 게 <> 안에 있는 개체를 가리키지 않으면 cast 실패 후 null 반환, () 안에 전달하려는 항목이 null 이면 cast 실패하고 null 반환
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
	Super::NativeUpdateAnimation(DeltaTIime);  //DeltaTime을 부모 ver로 전달?

	if (PlayerCharacterMovement)  // NULL이 아닌 경우에만 이 포인터에 엑세스
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(PlayerCharacterMovement->Velocity);  // UKismetMathLibrary 클래스 안에 있는 VSizeXY 같은 함수들은 static이므로 KismetMathLibrary 유형의 객체(object)가 존재할 필요가 없음, 이 함수는 상태나 클래스의 변수를 변경하지 않음, 그저 수학 계산 수행하고 결과 반환하는 일만 함
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
