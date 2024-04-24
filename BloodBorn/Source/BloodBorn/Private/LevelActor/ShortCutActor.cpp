// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelActor/ShortCutActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../BloodBornCharacter.h"
#include "LevelActor/BloodBornGameInstance.h"

// Sets default values
AShortCutActor::AShortCutActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(rootComp);

	moveStartArea = CreateDefaultSubobject<UBoxComponent>(TEXT("MoveCollsion"));
	moveStartArea->SetupAttachment(rootComp);
	moveStartArea->SetCollisionProfileName(FName("OverlapAllDynamic"));
	moveStartArea->SetGenerateOverlapEvents(true);
	moveStartArea->SetRelativeLocation(FVector(254.0f, -250.0f, 113.0f));
	moveStartArea->SetBoxExtent(FVector(150.0f, 150.0f, 32.0f));

	platform = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platfrom"));
	platform->SetupAttachment(rootComp);
}

// Called when the game starts or when spawned
void AShortCutActor::BeginPlay()
{
	Super::BeginPlay();
	
	moveStartArea->OnComponentBeginOverlap.AddDynamic(this, &AShortCutActor::MovePlatform);
	
	gi = Cast<UBloodBornGameInstance>(GetGameInstance());

	//게임 인스턴스에서 플랫폼 상태를 가져와서 위/아래 위치에 배치한다.
	if (gi != nullptr) {
		if (gi->platformState == EPlatformState::BOTTOM) {
			SetActorLocation(bottomPlace);
		}
		else if (gi->platformState == EPlatformState::TOP) {
			SetActorLocation(topPlace);
		}
	}
}

// Called every frame
void AShortCutActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (gi != nullptr) {
		//이동중 상태로 전환됬을때만 이동한다.
		if (gi->platformState == EPlatformState::MOVING) {
			currentTime = FMath::Min(currentTime + DeltaTime, movingTime);
			FVector newLoc = FMath::InterpEaseInOut(startLoc, targetLoc, currentTime / movingTime, 2.0f);
			SetActorLocation(newLoc);
			//도착하면 도착 위치를 참조하여 게임인스턴스의 플랫폼 상태를 갱신한다.
			if (currentTime == movingTime) {
				if (targetLoc == topPlace) {
					gi->platformState = EPlatformState::TOP;
				}
				else {
					gi->platformState = EPlatformState::BOTTOM;
				}
				currentTime = 0.0f;
			}
		}
	}
}

void AShortCutActor::MovePlatform(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABloodBornCharacter* player = Cast<ABloodBornCharacter>(OtherActor);
	//올라선 액터가 플레이어인지 확인
	if (player != nullptr) {
		//게임 인스턴스 방어코드
		if (gi != nullptr) {
			//바닥이 이동중이 아니라면
			if (gi->platformState != EPlatformState::MOVING) {
				//게임인스턴스의 플랫폼 상태를 참조하여 출발/도착점을 지정한다.
				if (gi->platformState == EPlatformState::BOTTOM) {
					startLoc = bottomPlace;
					targetLoc = topPlace;
				}
				else {
					startLoc = topPlace;
					targetLoc = bottomPlace;
				}
				//플랫폼이 이동한다.
				gi->platformState = EPlatformState::MOVING;
			}
		}
	}
}

