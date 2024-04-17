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
// 			// 몽타주 재생 안함
// 			return;
// 		}
	}
	// rate 도 0.8로 해야함, 닷지 불가능도 //
}
