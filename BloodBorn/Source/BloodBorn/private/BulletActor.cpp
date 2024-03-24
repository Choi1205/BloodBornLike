// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletActor.h"
#include "BloodBornCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABulletActor::ABulletActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bulletBody = CreateDefaultSubobject<USphereComponent>(TEXT("sphereCollision"));
	SetRootComponent(bulletBody);
	bulletBody->SetSphereRadius(50.0f);
	bulletBody->SetRelativeScale3D(FVector(0.05f));

	bulletShape = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("staticMesh"));
	bulletShape->SetupAttachment(bulletBody);
	bulletShape->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
}

// Called when the game starts or when spawned
void ABulletActor::BeginPlay()
{
	Super::BeginPlay();

	bulletBody->OnComponentBeginOverlap.AddDynamic(this, &ABulletActor::CallHit);
	
}

// Called every frame
void ABulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	lifeTimeStack += DeltaTime;
	if (lifeTimeStack > lifeTime) {
		Destroy();
	}

	SetActorLocation(GetActorLocation() + GetActorForwardVector() * bulletSpeed * DeltaTime);

}

void ABulletActor::CallHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerREF = Cast<ABloodBornCharacter>(OtherActor);

	if (PlayerREF) {
		UE_LOG(LogTemp, Warning, TEXT("Player Damaged"));
	}
	Destroy();
}