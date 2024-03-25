#pragma once
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_NormalAttacking UMETA(DisplayName = "NormalAttacking"),   // 이렇게 하면 강공격이랑 록온도 여기 들어가나?
	EAS_StrongAttacking UMETA(DisplayName = "StrongAttacking")
};
