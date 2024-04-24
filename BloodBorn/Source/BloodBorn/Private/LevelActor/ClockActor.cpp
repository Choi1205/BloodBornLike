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
	//ȸ������ ������ True�� �Ǿ� ȸ������
	if (bIsRotateStart) {
		//������ 3ȸ �ȵ� �Ÿ��� �κб����� �ڿ������� ����. ����/���κп��� Ease�� �����Ͽ� �ε巴�� �����ϰ� �ε巴�� �����
		if (rotRate < targetRot) {
			rotRate = FMath::Min(rotRate + DeltaTime, targetRot);
			clockFront->SetRelativeRotation(FRotator(0.0f, 0.0f, FMath::InterpEaseInOut(-720.0f, -30.0f, rotRate / targetRot, 2.0f)));
			clockBack->SetRelativeRotation(FRotator(0.0f, 0.0f, FMath::InterpEaseInOut(720.0f, 30.0f, rotRate / targetRot, 2.0f)));
			clockBlad1->SetRelativeRotation(FRotator(0.0f, 0.0f, FMath::InterpEaseInOut(320.0f, 30.0f, rotRate / targetRot, 2.0f)));
			clockBlad2->SetRelativeRotation(FRotator(0.0f, 0.0f, FMath::InterpEaseInOut(-220.0f, -30.0f, rotRate / targetRot, 2.0f)));
		}

		//���� ���� ó������, �������� 9������(8ȸ �ȵ����� 5ȸ°�� ������)�� ���� �� 6ȸ° �ȵ��Ӻ��� �ȵ��̸� ����.
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
			//�ȵ��̴� �̵� ����� ���� �κ�
			if (tickTokRate == 1.0f) {
				tickTokRate = 0.0f;
				bIsTickToking = false;
			}
		}
	}
}

//�ȵ��� �̵��Ÿ� �����Լ�
void AClockActor::MoveTickTok()
{
	//���� �����Ӻ��� �ȵ��̸� ����
	bIsTickToking = true;
	//�������ڸ��� ���ϸ� Tick�� ����� �������� �� �� �־� �ް����� ���ϴ�
	tickTokCounter++;

	//�ȵ��� Ƚ���� ��ü �������� �����Ѵ�
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

