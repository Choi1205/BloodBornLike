// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "BloodVial.generated.h"

/**
 * 
 */
UCLASS()
class BLOODBORN_API ABloodVial : public AItem
{
	GENERATED_BODY()
	
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:
	class ABloodBornCharacter* player;

	// void GetBVs();
	void Equip(/*USceneComponent* InParent,*//*AActor* NewOwner, APawn* NewInstigator*/ AActor* OtherActor);


	FORCEINLINE int32 GetBloodVials() const {return BloodVials;}

private:
	
	UPROPERTY(EditAnywhere, Category = "BloodVial Properties")
	int32 BloodVials;
};
