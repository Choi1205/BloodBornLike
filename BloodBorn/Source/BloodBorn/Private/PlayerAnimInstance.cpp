// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "PlayerAnimInstance.h"
#include "BloodBorn/BloodBornCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// ���� �ʱ�ȭ�ϰ� ���� ����
void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();  // override�� �������̱� ������ super, �θ� ver ȣ���ؼ� �θ𿡼� ���� ���� �Ͼ���� Ȯ��, �ִϸ��̼� �ν��Ͻ��� ���⼭ ����� ��

	PlayerCharacter = Cast<ABloodBornCharacter>(TryGetPawnOwner());  // () �ȿ� �ִ� �� <> �ȿ� �ִ� ��ü�� ����Ű�� ������ cast ���� �� null ��ȯ, () �ȿ� �����Ϸ��� �׸��� null �̸� cast �����ϰ� null ��ȯ
		if (PlayerCharacter)
		{
			PlayerCharacterMovement = PlayerCharacter->GetCharacterMovement();
		}
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
