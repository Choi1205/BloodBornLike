

#include "Components/AttributeComponent.h"
#include "BloodBorn/BloodBornCharacter.h"


UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = /*true*/false;

}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	//player = Cast<ABloodBornCharacter>(GetOwner());
}

void UAttributeComponent::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);

	// 플레이어가 입은 데미지 양을 회복 가능한 체력으로 설정
	RegainableHealth = Damage;
	PredictedHP = Health + RegainableHealth;

}

void UAttributeComponent::UseStamina(float StaminaCost)
{
	Stamina = FMath::Clamp(Stamina - StaminaCost, 0.0f, MaxStamina);
	if(StaminaCost > 49){
		staCoolDown = 2.0f;
	}
}

float UAttributeComponent::GetHealthPercent()
{
	return Health / MaxHealth;
}

float UAttributeComponent::GetStaminaPercent()
{
	return Stamina / MaxStamina;
}

float UAttributeComponent::GetRegainHealthPercent()
{
	return PredictedHP / MaxHealth;
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

	// 회복량이 회복 가능한 체력보다 클 경우, 회복 가능한 체력만큼만 회복
	RegainHP = FMath::Min(RegainableHealth, RegainHP);

	// 현재 체력을 회복량만큼 증가
	Health = FMath::Min(Health + RegainHP, MaxHealth);

	// 회복 가능한 체력 갱신
	RegainableHealth -= RegainHP;


}

void UAttributeComponent::ResetRegainableHealth()
{
	RegainableHealth = 0.0f;
	PredictedHP = Health;

}

void UAttributeComponent::AddBloodVials(int32 AmountOfBVial)
{
	BloodVials += AmountOfBVial;
	//BloodVials = FMath::Clamp(BloodVials + AmountOfBVial, 0, 20);
}

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
