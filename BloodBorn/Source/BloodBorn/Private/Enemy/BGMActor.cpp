// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BGMActor.h"
#include "Components/AudioComponent.h"

// Sets default values
ABGMActor::ABGMActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(rootComp);

	audioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	audioComp->SetupAttachment(rootComp);
	audioComp->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void ABGMActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABGMActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (startTime > 0.0f) {
		startTime -= DeltaTime;
		if (startTime <= 0.0f) {
			audioComp->Play();
		}
	}
	if (bIsdead) {
		volume -= DeltaTime * 0.3f;
		audioComp->SetVolumeMultiplier(volume);
	}
}

void ABGMActor::slowKillThis()
{
	bIsdead = true;
	FTimerHandle killTimer;
	GetWorldTimerManager().SetTimer(killTimer, FTimerDelegate::CreateLambda([&]() {
		Destroy();
		}), 3.0f, false);
}

