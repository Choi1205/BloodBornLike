// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BloodDecalActor.h"
#include "Components/DecalComponent.h"

//바닥에 생성됨
void ABloodDecalActor::BeginPlay()
{
	int32 num = FMath::RandRange(0, bloodArray.Num()-1);
	SetDecalMaterial(bloodArray[num]);
	GetDecal()->DecalSize = FVector(10.0f, 256.0f, 256.0f);
}

//플레이어에게 붙임
void ABloodDecalActor::ForAttachPlayer()
{
	GetDecal()->DecalSize = FVector(70.0f, 70.0f, 120.0f);
}