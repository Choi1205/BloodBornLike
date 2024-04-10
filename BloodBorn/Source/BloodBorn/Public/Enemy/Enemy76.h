

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enemy76.generated.h"

class UAttributeComponent;

UCLASS()
class BLOODBORN_API AEnemy76 : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy76();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GotParryAttackCPP(float damage);
	virtual bool GetInStun() override;
	virtual float GetHealth() override;

protected:

	virtual void BeginPlay() override;

public:	

private:
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

};
