#pragma once

#include "CoreMinimal.h"
#include "ItemStats.generated.h"

USTRUCT(BlueprintType)
struct FItemStats
{
	GENERATED_USTRUCT_BODY();
	
	UPROPERTY()
	float PhysicalDamage = 0.0f;
	
	UPROPERTY()
	float MysticDamage = 0.0f;

	UPROPERTY()
	float DamageShield = 0.0f;

	UPROPERTY()
	float Strength = 0.0f;
	
	UPROPERTY()
	float Stamina = 0.0f;
	
	UPROPERTY()
	float MovementSpeed = 0.0f;
	
	UPROPERTY()
	float JumpHeight = 0.0f;

	UPROPERTY()
	float Mobility = 0.0f;

	UPROPERTY()
	float Stealth = 0.0f;

	UPROPERTY()
	float Awareness = 0.0f;

	UPROPERTY()
	float FlyTime = 0.0f;

	UPROPERTY()
	float Longevity = 0.0f;
};