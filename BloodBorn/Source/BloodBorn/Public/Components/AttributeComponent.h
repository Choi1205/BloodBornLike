
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLOODBORN_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void RegenStamina(float DeltaTime);
	void DeclineStamina(float DeltaTime);

	// Current Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes")
	float HealthSlider;

	////////////////////////////////////////s
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes")
	float RegainableHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes")
	float /*MaxRegainableHealth*/PredictedHP;  // UI상 health + regainableHP

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes")
	float Stamina;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes")
	float MaxStamina;

	// 천천히 줄어드는 스테미너
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes")
	float DecreaseStamina;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes")
	float StaminaSlider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes")
	int32 BloodVial = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes")
	int32 Bullet = 20;

// 	UPROPERTY(EditAnywhere)
// 	class UNiagaraSystem* healEffect;
// 
// 	UPROPERTY(EditAnywhere)
// 	class UNiagaraComponent* instanceEffect;

protected:
	virtual void BeginPlay() override;



private:

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float DodgeCost = 15.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float NormalAttackCost = 50.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float HeavyAttackCost = 70.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float StaminaRegenRate = 8.f; // 초당 생성

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float StaminaDecreaseRate = 10.0f;

	UPROPERTY(VisibleAnywhere, Category = "Actor Attributes")
	float LastDamageReceived;

	//class ABloodBornCharacter* player;

	float staCoolDown = 0.0;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float RegainHPDecreaseRate = 8.f; // 초당

public:	
	void ReceiveDamage(float Damage);
	void UseStamina(float StaminaCost);

	float GetHealthPercent();
	float GetStaminaPercent();

	float GetHealthSlider();
	float GetStaminaSlider();

	float GetRegainHealthPercent();
	float GetDecreaseStaminaPercent();

	//float MaxRegainableHealth;

	void RegainHealth(float RegainHP);
	void ResetRegainableHealth();

	void UseBloodVial(int32 BV);
	void UseBullet(int32 bullet);
	void MakeBullet(int32 newBullet);
	// void AddBloodVials(int32 AmountOfBVial);

	FORCEINLINE float GetDodgeCost() const { return DodgeCost; }
	FORCEINLINE float GetNormalAttackCost() const { return NormalAttackCost; }
	FORCEINLINE float GetHeavyAttackCost() const { return HeavyAttackCost; }
	FORCEINLINE float GetStamina() const { return Stamina; }
	FORCEINLINE float GetHealth() const { return Health; }
	///
	FORCEINLINE float GetHealthSlider() const { return HealthSlider; }
	FORCEINLINE float GetStaminaSlider() const { return StaminaSlider; }
	///
	FORCEINLINE float GetLastDamageReceived() const { return LastDamageReceived; }

	FORCEINLINE float GetDecreaseStamina() const { return DecreaseStamina; }



	FORCEINLINE float GetRegainableHealth() const { return RegainableHealth; }
	FORCEINLINE float GetPredictedHP() const { return PredictedHP; }


	FORCEINLINE int32 GetBloodVial() const { return BloodVial; }
	FORCEINLINE int32 GetBullet() const { return Bullet; }

};
