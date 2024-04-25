// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Gun.h"
#include "BloodBorn/BloodBornCharacter.h"
#include "../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h"
#include "../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
#include "HUD/WeaponOverlay.h"
#include "BloodBornGameMode.h"
#include "LevelActor/BloodBornGameInstance.h"


AGun::AGun()
{
	gunEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EquipGun Effect"));
	gunEffect->SetupAttachment(GetRootComponent());
	gunEffect->bAutoActivate = false;

	fireEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Fire Effect"));
	fireEffect->SetupAttachment(GetRootComponent());
	fireEffect->bAutoActivate = false;
}

void AGun::BeginPlay()
{
	Super::BeginPlay();
	
	gunEffect->Activate(true);
}

void AGun::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	ABloodBornGameMode* gameMode = Cast<ABloodBornGameMode>(GetWorld()->GetAuthGameMode());
	gameMode->ShowWeaponUI(false);
}

void AGun::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	ABloodBornGameMode* gameMode = Cast<ABloodBornGameMode>(GetWorld()->GetAuthGameMode());
	gameMode->HideWeaponUI();
}

void AGun::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
	ItemState = EItemState::EIS_Equipped;
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// player = Cast<ABloodBornCharacter>(NewOwner);

	Cast<UBloodBornGameInstance>(GetGameInstance())->bHadGun = true;
}
