

#include "Components/AttributeComponent.h"
#include "BloodBorn/BloodBornCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"



UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = /*true*/false;


}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UAttributeComponent::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	HealthSlider = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	// �÷��̾ ���� ������ ���� ȸ�� ������ ü������ ����
	RegainableHealth = Damage;
	PredictedHP = Health + RegainableHealth;

}

void UAttributeComponent::UseStamina(float StaminaCost)
{
	Stamina = FMath::Clamp(Stamina - StaminaCost, 0.0f, MaxStamina);
	if(StaminaCost > 49){
		staCoolDown = 2.0f;
	}
	StaminaSlider = FMath::Clamp(Stamina - StaminaCost, 0.0f, MaxStamina);
}

float UAttributeComponent::GetHealthPercent()
{
	return Health / MaxHealth;
}

float UAttributeComponent::GetStaminaPercent()
{
	return Stamina / MaxStamina;
}

float UAttributeComponent::GetHealthSlider()
{
	return Health / MaxHealth;
}

float UAttributeComponent::GetStaminaSlider()
{
	return Stamina / MaxStamina;
}

float UAttributeComponent::GetRegainHealthPercent()
{
	return PredictedHP / MaxHealth;
}

float UAttributeComponent::GetDecreaseStaminaPercent()
{
	return DecreaseStamina / MaxStamina;
}

void UAttributeComponent::RegainHealth(float RegainHP)
{
	//if (RegainHP > 0.f )
	//{
		//MaxRegainableHealth = FMath::Min(MaxHealth, Health + RegainHP);
		//UE_LOG(LogTemp, Warning, TEXT("MaxRegainableHealth: %f"), MaxRegainableHealth);
		//Health = FMath::Clamp(Health + RegainHP, 0.0f, MaxRegainableHealth);
	//}
	if (RegainableHealth <= 0.0f || RegainHP <= 0.0f)
	{
		return;
	}

	// ȸ������ ȸ�� ������ ü�º��� Ŭ ���, ȸ�� ������ ü�¸�ŭ�� ȸ��
	RegainHP = FMath::Min(RegainableHealth, RegainHP);

	// ���� ü���� ȸ������ŭ ����
	Health = FMath::Min(Health + RegainHP, MaxHealth);

	// ȸ�� ������ ü�� ����
	RegainableHealth -= RegainHP;



}

void UAttributeComponent::ResetRegainableHealth()
{
	RegainableHealth = 0.0f;
	PredictedHP = Health;

}

void UAttributeComponent::UseBloodVial(int32 BV)
{
	BloodVial -= 1;
	//Health += 100;
	Health = FMath::Clamp(Health + 100, 0.f, MaxHealth);

}

void UAttributeComponent::UseBullet(int32 bullet)
{
	Bullet -= 1;
}

void UAttributeComponent::MakeBullet(int32 newBullet)
{
	// Bullet += 5;
	Bullet = FMath::Clamp(Bullet + 5, 0.f, 20);
	// ���� newBullet�� hp?
	Health = FMath::Clamp(Health - 100, 0.f, MaxHealth);  // Ŭ������ ��� �� Min�� ��ߵ�..
}

// void UAttributeComponent::AddBloodVials(int32 AmountOfBVial)
// {
// 	BloodVials += AmountOfBVial;
// 	//BloodVials = FMath::Clamp(BloodVials + AmountOfBVial, 0, 20);
// }

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAttributeComponent::RegenStamina(float DeltaTime)
{
	if (staCoolDown > 0.0f) {
		staCoolDown -= DeltaTime;
	}
	if (staCoolDown <= 0.f) {
		Stamina = FMath::Clamp(Stamina + StaminaRegenRate * DeltaTime, 0.0f, MaxStamina);
	}
}

void UAttributeComponent::DeclineStamina(float DeltaTime)
{
	DecreaseStamina = FMath::Clamp(Stamina - StaminaDecreaseRate * DeltaTime, 0.0f, MaxStamina);  // 0.0f�� �ƴ϶� Stamina? 
}
