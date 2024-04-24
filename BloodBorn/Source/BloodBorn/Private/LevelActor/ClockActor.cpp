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
	clockInner->SetRelativeRotation(FRotator(0.0f, 0.0f, -240.0f));

	clockBack = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClockBack"));
	clockBack->SetupAttachment(rootComp);
	clockBack->SetRelativeRotation(FRotator(0.0f, 0.0f, 720.0f));

	clockBlad1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClockBlad1"));
	clockBlad1->SetupAttachment(rootComp);
	clockBlad1->SetRelativeRotation(FRotator(0.0f, 0.0f, 320.0f));

	clockBlad2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClockBlad2"));
	clockBlad2->SetupAttachment(rootComp);
	clockBlad2->SetRelativeRotation(FRotator(0.0f, 0.0f, -220.0f));

}

// Called when the game starts or when spawned
void AClockActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AClockActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//회전시작 변수가 True가 되야 회전시작
	if (bIsRotateStart) {
		//마지막 3회 똑딱 거리는 부분까지는 자연스럽게 돈다. 시작/끝부분에는 Ease를 적용하여 부드럽게 시작하고 부드럽게 멈춘다
		if (rotRate < targetRot) {
			rotRate = FMath::Min(rotRate + DeltaTime, targetRot);
			clockFront->SetRelativeRotation(FRotator(0.0f, 0.0f, FMath::InterpEaseInOut(-720.0f, -30.0f, rotRate / targetRot, 2.0f)));
			clockBack->SetRelativeRotation(FRotator(0.0f, 0.0f, FMath::InterpEaseInOut(720.0f, 30.0f, rotRate / targetRot, 2.0f)));
			clockBlad1->SetRelativeRotation(FRotator(0.0f, 0.0f, FMath::InterpEaseInOut(320.0f, 30.0f, rotRate / targetRot, 2.0f)));
			clockBlad2->SetRelativeRotation(FRotator(0.0f, 0.0f, FMath::InterpEaseInOut(-220.0f, -30.0f, rotRate / targetRot, 2.0f)));
		}

		//안쪽 고리는 처음부터, 나머지는 9초지점(8회 똑딱임중 5회째가 끝날때)에 정지 후 6회째 똑딱임부터 똑딱이며 돈다.
		if (bIsTickToking) {
			tickTokRate = FMath::Min(tickTokRate + DeltaTime, 1.0f);
			if (tickTokCounter < 9) {
				clockInner->SetRelativeRotation(FRotator(0.0f, 0.0f, FMath::Lerp(prevTickTokRot, nextTickTokRot, tickTokRate)));
			}
			if (tickTokCounter > 5 && tickTokCounter < 9) {
				clockFront->SetRelativeRotation(FRotator(0.0f, 0.0f, FMath::Lerp(prevFrontRot, nextFrontRot, tickTokRate)));
				clockBack->SetRelativeRotation(FRotator(0.0f, 0.0f, FMath::Lerp(prevBackRot, nextBackRot, tickTokRate)));
				clockBlad1->SetRelativeRotation(FRotator(0.0f, 0.0f, FMath::Lerp(prevBlad1Rot, nextBlad1Rot, tickTokRate)));
				clockBlad2->SetRelativeRotation(FRotator(0.0f, 0.0f, FMath::Lerp(prevBlad2Rot, nextBlad2Rot, tickTokRate)));
			}
			else if (tickTokCounter > 5) {
				clockBlad1->SetRelativeRotation(FRotator(0.0f, 0.0f, FMath::Lerp(prevBlad1Rot, nextBlad1Rot, tickTokRate)));
				clockBlad2->SetRelativeRotation(FRotator(0.0f, 0.0f, FMath::Lerp(prevBlad2Rot, nextBlad2Rot, tickTokRate)));
			}
			//똑딱이는 이동 종료시 리셋 부분
			if (tickTokRate == 1.0f) {
				tickTokRate = 0.0f;
				bIsTickToking = false;
			}
		}
	}
}

//똑딱임 이동거리 제어함수
void AClockActor::MoveTickTok()
{
	//다음 프레임부터 똑딱이며 돈다
	bIsTickToking = true;
	//시작하자마자 더하면 Tick의 제어와 같은수를 쓸 수 있어 햇갈림이 덜하다
	tickTokCounter++;

	//똑딱인 횟수로 전체 움직임을 제어한다
	if (tickTokCounter < 9) {
		prevTickTokRot = clockInner->GetRelativeRotation().Roll;
		nextTickTokRot = prevTickTokRot + 30.0f;
		if (tickTokCounter > 5) {
			prevFrontRot = clockFront->GetRelativeRotation().Roll;
			prevBackRot = clockBack->GetRelativeRotation().Roll;
			nextFrontRot = prevFrontRot + 10.0f;
			nextBackRot = prevBackRot - 10.0f;

			prevBlad1Rot = clockBlad1->GetRelativeRotation().Roll;
			prevBlad2Rot = clockBlad2->GetRelativeRotation().Roll;
			nextBlad1Rot = prevBlad1Rot - 10.0f;
			nextBlad2Rot = prevBlad2Rot + 10.0f;
		}
		GetWorld()->GetTimerManager().SetTimer(tickTokHandle, FTimerDelegate::CreateLambda([&]() {
			MoveTickTok();
			}), 2.0f, false);
	}
	else {
		prevBlad1Rot = clockBlad1->GetRelativeRotation().Roll;
		prevBlad2Rot = clockBlad2->GetRelativeRotation().Roll;
		nextBlad1Rot = prevBlad1Rot - 10.0f;
		nextBlad2Rot = prevBlad2Rot + 10.0f;
	}
}

