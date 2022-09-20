#pragma once

#define	TRACE_LENGTH 50000.f

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	Unoccupied UMETA(DisplayName = "Unoccupied"),
	Melee UMETA(DisplayName = "Melee"),
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	AK47 UMETA(DisplayName = "AK47"),
	Axe UMETA(DisplayName = "Axe")
};