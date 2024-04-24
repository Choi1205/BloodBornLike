// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/BBPlayerHUD.h"
#include "HUD/PlayerOverlay.h"
#include "HUD/DieOverlay.h"
#include "HUD/WeaponOverlay.h"

// void ABBPlayerHUD::BeginPlay()
// {
// 	Super::BeginPlay();
// 
// 	UWorld* world = GetWorld();
// 	if (world)
// 	{
// 		APlayerController* controller = world->GetFirstPlayerController();
// 		if (controller && PlayerOverlayClass)
// 		{
// 			// UPlayerOverlay* playerOverlay = CreateWidget<UPlayerOverlay>(controller, PlayerOverlayClass); // ����� �������� �����ؼ� �ּ�ó���� ȣ���� ���� ����
// 			PlayerOverlay = CreateWidget<UPlayerOverlay>(controller, PlayerOverlayClass);
// 			PlayerOverlay->AddToViewport();
// 		}
// 	}
// }

void ABBPlayerHUD::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UWorld* world = GetWorld();
	if (world)
	{
		APlayerController* controller = world->GetFirstPlayerController();
		if (controller && PlayerOverlayClass)
		{
			// UPlayerOverlay* playerOverlay = CreateWidget<UPlayerOverlay>(controller, PlayerOverlayClass); ����� �������� �����ؼ� �ּ�ó���� ȣ���� ���� ����
			PlayerOverlay = CreateWidget<UPlayerOverlay>(controller, PlayerOverlayClass);
			PlayerOverlay->AddToViewport();
		}
	}
}

void ABBPlayerHUD::ShowDieOverlay()
{
	UWorld* world = GetWorld();
	if (world)
	{
		APlayerController* controller = world->GetFirstPlayerController();
		if (controller && DieOverlayClass)
		{
			DieOverlay = CreateWidget<UDieOverlay>(controller, DieOverlayClass);
			DieOverlay->AddToViewport();

			DieOverlay->PlayAnimationForward(DieOverlay->anim_died);
			DieOverlay->PlayAnimationForward(DieOverlay->anim_diedBack);
			DieOverlay->PlayAnimationForward(DieOverlay->anim_black);
		}
	}
}

void ABBPlayerHUD::showWeaponOverlay()
{
	UWorld* world = GetWorld();
	if (world)
	{
		APlayerController* controller = world->GetFirstPlayerController();
		if (controller && WeaponOverlayClass)
		{
			WeaponOverlay = CreateWidget<UWeaponOverlay>(controller, WeaponOverlayClass);
			WeaponOverlay->AddToViewport();
		}
	}
}
