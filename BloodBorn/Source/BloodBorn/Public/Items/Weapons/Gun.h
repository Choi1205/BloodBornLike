// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Gun.generated.h"

/**
 * 
 */
UCLASS()
class BLOODBORN_API AGun : public AItem
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
											int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:
	AGun();
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);

	class ABloodBornCharacter* player;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* gunEffect;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* equipGunOverlay;

	class UWeaponOverlay* weaponOverlay;


};
