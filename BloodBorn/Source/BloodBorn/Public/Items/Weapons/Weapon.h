// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "BloodBorn/CharacterTypes.h"
#include "Weapon.generated.h"

DECLARE_EVENT_TwoParams(AWeapon, FSuccessfulAttackEvent, float, EAttackType);

class UBoxComponent;

/**
 * 
 */
UCLASS()
class BLOODBORN_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);

	TArray<AActor*> IgnoreActors;

	FSuccessfulAttackEvent OnSuccessfulAttackEvent;

	class ABloodBornCharacter* player; 

	class AItem* item;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* sawEffect;

protected:
	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, Category = "WeaponSaw Properties")
	UBoxComponent* SawBox;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	UPROPERTY(EditAnywhere, Category = "WeaponSaw Properties")
	float Damage = 20.f;

public:
	FORCEINLINE UBoxComponent* GetSawBox() const { return SawBox; }

	//float ApplyDamage(AActor* DamagedActor);  // 데미지 적용하고 데미지 반환
	//void ApplyDamage(AActor* DamagedActor, float DamageAmount, EAttackType AttackType);
	float ApplyDamage(AActor* DamagedActor, float DamageAmount, EAttackType AttackType);

};
