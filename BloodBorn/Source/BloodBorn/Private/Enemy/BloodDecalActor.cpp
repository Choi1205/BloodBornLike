// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BloodDecalActor.h"
#include "Components/DecalComponent.h"

void ABloodDecalActor::BeginPlay()
{
	int32 num = FMath::RandRange(0, bloodArray.Num()-1);
	SetDecalMaterial(bloodArray[num]);
	GetDecal()->DecalSize = FVector(4, 256, 256);
}

void ABloodDecalActor::ForAttachPlayer()
{
	GetDecal()->DecalSize = FVector(100.0f, 100.0f, 120.0f);
}