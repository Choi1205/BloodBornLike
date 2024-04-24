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

	//���� �ν��Ͻ����� �÷��� ���¸� �����ͼ� ��/�Ʒ� ��ġ�� ��ġ�Ѵ�.
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
		//�̵��� ���·� ��ȯ�������� �̵��Ѵ�.
		if (gi->platformState == EPlatformState::MOVING) {
			currentTime = FMath::Min(currentTime + DeltaTime, movingTime);
			FVector newLoc = FMath::InterpEaseInOut(startLoc, targetLoc, currentTime / movingTime, 2.0f);
			SetActorLocation(newLoc);
			//�����ϸ� ���� ��ġ�� �����Ͽ� �����ν��Ͻ��� �÷��� ���¸� �����Ѵ�.
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
	//�ö� ���Ͱ� �÷��̾����� Ȯ��
	if (player != nullptr) {
		//���� �ν��Ͻ� ����ڵ�
		if (gi != nullptr) {
			//�ٴ��� �̵����� �ƴ϶��
			if (gi->platformState != EPlatformState::MOVING) {
				//�����ν��Ͻ��� �÷��� ���¸� �����Ͽ� ���/�������� �����Ѵ�.
				if (gi->platformState == EPlatformState::BOTTOM) {
					startLoc = bottomPlace;
					targetLoc = topPlace;
				}
				else {
					startLoc = topPlace;
					targetLoc = bottomPlace;
				}
				//�÷����� �̵��Ѵ�.
				gi->platformState = EPlatformState::MOVING;
			}
		}
	}
}

