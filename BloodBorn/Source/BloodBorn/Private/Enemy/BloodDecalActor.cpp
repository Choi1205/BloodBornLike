// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BloodDecalActor.h"
#include "Components/DecalComponent.h"

void ABloodDecalActor::BeginPlay()
{
	int32 num = FMath::RandRange(0, bloodArray.Num()-1);
	SetDecalMaterial(bloodArray[num]);
	GetDecal()->DecalSize = FVector(10.0f, 256.0f, 256.0f);
}

void ABloodDecalActor::ForAttachPlayer()
{
	GetDecal()->DecalSize = FVector(70.0f, 70.0f, 120.0f);
}