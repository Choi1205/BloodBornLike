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
		CharacterState = PlayerCharacter->GetCharacterState();
		//ActionState = PlayerCharacter->GetActionState();
	}
}

void UPlayerAnimInstance::AnimNotify_Heal()
{

	UE_LOG(LogTemp, Warning, TEXT("ANIMNOTIFY_HEAL"));
	PlayerCharacter->UseBloodVial();
	if (/*Attributes && */PlayerOverlay)
	{
// 		if (Attributes->BloodVial > 0)
// 		{
			// ActionState = EActionState::EAS_Heal;
// 			if (ActionState == EActionState::EAS_Heal)
// 			{
				//PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 150.f;
				//PlayerCharacter->GetCharacterMovement()->MaxAcceleration = 512.f;
		UE_LOG(LogTemp, Warning, TEXT("PLAYEROVERLAY VALID"));

				Attributes->UseBloodVial(Attributes->GetBloodVial());
				PlayerOverlay->SetVial(Attributes->GetBloodVial());
				PlayerOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				PlayerOverlay->SetHealthSliderBarPercent(Attributes->GetHealthSlider());
//			}
//		}
// 		else
// 		{
// 			// ��Ÿ�� ��� ����
// 			return;
// 		}
	}
	// rate �� 0.8�� �ؾ���, ���� �Ұ��ɵ� //
}
