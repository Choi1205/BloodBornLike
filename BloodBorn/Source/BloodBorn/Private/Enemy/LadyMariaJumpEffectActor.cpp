// Fill out your copyright notice in the Description page of Project Settings.
//보스가 점프공격을 할 때 회전하면서 피 이펙트를 뿌리는 액터.

#include "Enemy/LadyMariaJumpEffectActor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/RotatingMovementComponent.h>
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>

// Sets default values
ALadyMariaJumpEffectActor::ALadyMariaJumpEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("rootComp"));
	SetRootComponent(rootComp);

	rotatingComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingComp"));
	rotatingComp->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	swordTrail1_1 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("swordTrail1_1"));
	swordTrailArray.Add(swordTrail1_1);
	swordTrail1_2 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("swordTrail1_2"));
	swordTrailArray.Add(swordTrail1_2);
	swordTrail2_1 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("swordTrail2_1"));
	swordTrailArray.Add(swordTrail2_1);
	swordTrail2_2 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("swordTrail2_2"));
	swordTrailArray.Add(swordTrail2_2);

	for(int32 i = 0; i < 4; i++) {
		swordTrailArray[i]->SetupAttachment(rootComp);
		swordTrailArray[i]->SetAutoActivate(false);
		if(i % 2 == 0){
			swordTrailArray[i]->SetRelativeLocation(FVector(0.0f, 100.0f, 0.0f));
			swordTrailArray[i]->SetRelativeRotation(FRotator(30.0f, -90.0f, 0.0f));
		}
		else{
			swordTrailArray[i]->SetRelativeLocation(FVector(0.0f, -100.0f, 0.0f));
			swordTrailArray[i]->SetRelativeRotation(FRotator(30.0f, 90.0f, 0.0f));
		}
	}

	swordBurst1_1 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("swordBurst1_1"));
	swordBurstArray.Add(swordBurst1_1);
	swordBurst1_2 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("swordBurst1_2"));
	swordBurstArray.Add(swordBurst1_2);
	swordBurst2_1 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("swordBurst2_1"));
	swordBurstArray.Add(swordBurst2_1);
	swordBurst2_2 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("swordBurst2_2"));
	swordBurstArray.Add(swordBurst2_2);

	for (int32 i = 0; i < 4; i++) {
		swordBurstArray[i]->SetupAttachment(rootComp);
		swordBurstArray[i]->SetAutoActivate(false);
		if (i % 2 == 0) {
			swordBurstArray[i]->SetRelativeLocation(FVector(0.0f, 100.0f, 0.0f));
			swordBurstArray[i]->SetRelativeRotation(FRotator(-30.0f, 90.0f, 0.0f));
		}
		else {
			swordBurstArray[i]->SetRelativeLocation(FVector(0.0f, -100.0f, 0.0f));
			swordBurstArray[i]->SetRelativeRotation(FRotator(-30.0f, -90.0f, 0.0f));
		}
	}

}

// Called when the game starts or when spawned
//void ALadyMariaJumpEffectActor::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}

// Called every frame
void ALadyMariaJumpEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsJumping) {
		swordTrailTimer1 += DeltaTime;
		if (swordTrailTimer1 > swordTrailActiveTime) {
			swordTrailArray[0]->Activate(true);
			swordTrailArray[1]->Activate(true);
			swordTrailTimer1 = 0.0f;
		}
		swordTrailTimer2 += DeltaTime;
		if (swordTrailTimer2 > swordTrailActiveTime) {
			swordTrailArray[2]->Activate(true);
			swordTrailArray[3]->Activate(true);
			swordTrailTimer2 = 0.0f;
		}
		swordBurstTimer1 += DeltaTime;
		if (swordBurstTimer1 > swordBurstActiveTime) {
			swordBurstArray[0]->Activate(true);
			swordBurstArray[1]->Activate(true);
			swordBurstTimer1 = 0.0f;
		}
		swordBurstTimer2 += DeltaTime;
		if (swordBurstTimer2 > swordBurstActiveTime) {
			swordBurstArray[2]->Activate(true);
			swordBurstArray[3]->Activate(true);
			swordBurstTimer2 = 0.0f;
		}
	}
}

void ALadyMariaJumpEffectActor::JumpingToggle()
{
	//실행될때마다 트루와 펄스가 번갈아가며 바뀐다. 
	bIsJumping = !bIsJumping;
}

