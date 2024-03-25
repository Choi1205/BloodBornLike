// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "BloodBorn/BloodBornCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/GamePlayStatics.h"


AWeapon::AWeapon()
{
	SawBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Saw Box"));
	SawBox->SetupAttachment(GetRootComponent());
	SawBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SawBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SawBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	SawBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();  // 월드로케이션
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	FHitResult BoxHit;
	UKismetSystemLibrary::BoxTraceSingle( 
			this, Start, End,
			FVector(5.f, 5.f, 5.f), BoxTraceStart->GetComponentRotation(), ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore,
			EDrawDebugTrace::ForDuration, BoxHit, true
	);
	
	if (BoxHit.GetActor())
	{

		IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());  // enemy가 지금 IHitInterface 객체니 캐스트 가능
		
		if (HitInterface)
		{
			HitInterface->GetHit(BoxHit.ImpactPoint);
		}
		IgnoreActors.AddUnique(BoxHit.GetActor());

		UGameplayStatics::ApplyDamage(
				BoxHit.GetActor(),
				Damage,
				GetInstigator()->GetController(),
				this,
				UDamageType::StaticClass()
		);
	}
}
