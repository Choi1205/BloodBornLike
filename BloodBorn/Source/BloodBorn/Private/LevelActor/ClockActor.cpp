// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelActor/ClockActor.h"
#include "MathUtil.h"

// Sets default values
AClockActor::AClockActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(rootComp);

	clockCase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClockCase"));
	clockCase->SetupAttachment(rootComp);

	clockFront = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClockFront"));
	clockFront->SetupAttachment(rootComp);
	clockFront->SetRelativeRotation(FRotator(0.0f, 0.0f, -720.0f));

	clockInner = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClockInner"));
	clockInner->SetupAttachment(rootComp);
	clockInner->SetRelativeRotation(FRotator(0.0f, 0.0f, -80.0f));

	clockBack = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClockBack"));
	clockBack->SetupAttachment(rootComp);
	clockBack->SetRelativeRotation(FRotator(0.0f, 0.0f, 720.0f));

	clockBlad1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClockBlad1"));
	clockBlad1->SetupAttachment(rootComp);
	clockBlad1->SetRelativeRotation(FRotator(0.0f, 0.0f, -40.0f));

	clockBlad2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClockBlad2"));
	clockBlad2->SetupAttachment(rootComp);
	clockBlad2->SetRelativeRotation(FRotator(0.0f, 0.0f, 140.0f));

}

// Called when the game starts or when spawned
void AClockActor::BeginPlay()
{
	Super::BeginPlay();

	MoveTickTok();
}

// Called every frame
void AClockActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(frontRotRate < frontTargetRot){
		frontRotRate = FMath::Min(frontRotRate + DeltaTime, frontTargetRot);
		clockFront->SetRelativeRotation(FRotator(0.0f, 0.0f, FMath::InterpEaseInOut(-720.0f, 0.0f, frontRotRate / frontTargetRot, 3.0f)));
		clockBack->SetRelativeRotation(FRotator(0.0f, 0.0f, FMath::InterpEaseInOut(720.0f, 0.0f, frontRotRate / frontTargetRot, 3.0f)));
	}
	else if (frontRotRate == frontTargetRot) {
		clockFront->SetRelativeRotation(FRotator(0.0f));
		clockBack->SetRelativeRotation(FRotator(0.0f));
	}

	if (bIsTickToking) {
		UE_LOG(LogTemp, Warning, TEXT("%d"), tickTokCounter);
		tickTokRate = FMath::Min(tickTokRate + DeltaTime, 1.0f);
		clockInner->SetRelativeRotation(FRotator(0.0f, 0.0f, FMath::Lerp(prevTickTokRot, nextTickTokRot, tickTokRate)));
		clockBlad1->SetRelativeRotation(FRotator(0.0f, 0.0f, FMath::Lerp(prevBlad1Rot, nextBlad1Rot, tickTokRate)));
		clockBlad2->SetRelativeRotation(FRotator(0.0f, 0.0f, FMath::Lerp(prevBlad2Rot, nextBlad2Rot, tickTokRate)));
	}

	if (tickTokRate == 1.0f) {
		tickTokRate = 0.0f;
		bIsTickToking = false;
	}
}

void AClockActor::MoveTickTok()
{
	bIsTickToking = true;
	tickTokCounter++;

	prevTickTokRot = clockInner->GetRelativeRotation().Roll;
	nextTickTokRot = prevTickTokRot + 10.0f;

	prevBlad1Rot = clockBlad1->GetRelativeRotation().Roll;
	prevBlad2Rot = clockBlad2->GetRelativeRotation().Roll;
	if (tickTokCounter < 6) {
		nextBlad1Rot = prevBlad1Rot - 10.0f;
		nextBlad2Rot = prevBlad2Rot - 10.0f;
	}
	else {
		nextBlad1Rot = prevBlad1Rot + 30.0f;
		nextBlad2Rot = prevBlad2Rot - 30.0f;
	}
	if (tickTokCounter < 8) {
		GetWorld()->GetTimerManager().SetTimer(tickTokHandle, FTimerDelegate::CreateLambda([&]() {
			MoveTickTok();
			}), 2.0f, false);
	}
}

