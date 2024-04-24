// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Gun.h"
#include "BloodBorn/BloodBornCharacter.h"
#include "../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h"
#include "../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
#include "HUD/WeaponOverlay.h"


AGun::AGun()
{
	gunEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EquipGun Effect"));
	gunEffect->SetupAttachment(GetRootComponent());
	gunEffect->bAutoActivate = false;
// 
// 	equipGunOverlay = CreateDefaultSubobject<UWidgetComponent>(TEXT("EquipGun Widget"));
// 	equipGunOverlay->SetupAttachment(GetRootComponent());
// 	equipGunOverlay->SetWidgetSpace(EWidgetSpace::Screen);
// 	equipGunOverlay->SetVisibility(false);
}

void AGun::BeginPlay()
{
	Super::BeginPlay();
	
	gunEffect->Activate(true);
}

void AGun::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (weaponOverlay)
	{
		weaponOverlay->img_Gun->SetVisibility(ESlateVisibility::Visible);
	}
}

void AGun::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	/*equipGunOverlay->SetVisibility(false);*/
	if (weaponOverlay)
	{
	weaponOverlay->img_Gun->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AGun::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	//equipGunOverlay->DestroyComponent(true);
	if (weaponOverlay)
	{
	weaponOverlay->img_Gun->SetVisibility(ESlateVisibility::Hidden);
	}
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
	ItemState = EItemState::EIS_Equipped;
	// player = Cast<ABloodBornCharacter>(NewOwner);
}
