// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/BBPlayerHUD.h"
#include "HUD/PlayerOverlay.h"

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
// 			// UPlayerOverlay* playerOverlay = CreateWidget<UPlayerOverlay>(controller, PlayerOverlayClass); // 헤더에 전역변수 생성해서 주석처리함 호옥시 몰라서 남김
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
			// UPlayerOverlay* playerOverlay = CreateWidget<UPlayerOverlay>(controller, PlayerOverlayClass); 헤더에 전역변수 생성해서 주석처리함 호옥시 몰라서 남김
			PlayerOverlay = CreateWidget<UPlayerOverlay>(controller, PlayerOverlayClass);
			PlayerOverlay->AddToViewport();
		}
	}
}
