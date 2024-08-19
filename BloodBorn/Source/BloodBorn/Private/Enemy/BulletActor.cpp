// Fill out your copyright notice in the Description page of Project Settings.
//에너미 원거리 공격 공용 탄환

#include "Enemy/BulletActor.h"
#include "BloodBorn/BloodBornCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

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

	particleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Component"));
	particleComp->SetupAttachment(bulletBody);
	particleComp->SetWorldScale3D(FVector(10.0f));
	particleComp->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void ABulletActor::BeginPlay()
{
	Super::BeginPlay();

	bulletBody->OnComponentBeginOverlap.AddDynamic(this, &ABulletActor::CallHit);

	//3초뒤 파괴
	SetLifeSpan(3.0f);

	particleComp->Activate(true);
}

// 1초당 speed값 만큼 날아간다
void ABulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime, true);

}

void ABulletActor::SetBulletSpeed(float bulletSpeed)
{
	Speed = bulletSpeed;
}

void ABulletActor::SetFirePower(float bulletDamage)
{
	damage = bulletDamage;
}

void ABulletActor::CallHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerREF = Cast<ABloodBornCharacter>(OtherActor);

	if (PlayerREF) {
		UGameplayStatics::ApplyDamage(PlayerREF, damage, nullptr, this, UDamageType::StaticClass());
	}

	Destroy();
}