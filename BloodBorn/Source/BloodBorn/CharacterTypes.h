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
	EAS_NormalAttacking UMETA(DisplayName = "NormalAttacking"),   // �̷��� �ϸ� �������̶� �Ͽµ� ���� ����?
	EAS_StrongAttacking UMETA(DisplayName = "StrongAttacking")
};
