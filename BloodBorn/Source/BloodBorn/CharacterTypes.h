#pragma once
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon"),
	ECS_LockOn UMETA(DisplayName = "LockOn"),
	//ECS_UnLockOn UMETA(DisplayName = "UnLockOn")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_HitReaction UMETA(DisplayName = "HitReaction"),
	EAS_NormalAttacking UMETA(DisplayName = "NormalAttacking"),
	EAS_StrongAttacking UMETA(DisplayName = "StrongAttacking"),
	EAS_HoldAttacking UMETA(DisplayName = "HoldAttacking"),
	EAS_Dodge UMETA(DisplayName = "Dodge"),
	EAS_GunFire UMETA(DisplayName = "GunFire"),
	EAS_Heal UMETA(DisplayName = "Heal"),
	EAS_MakeBullet UMETA(DisplayName = "MakeBullet"),
	EAS_Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	Normal,
	Heavy,
	HoldAttack
};
