#pragma once

#include "ItemTypes.generated.h"

USTRUCT(BlueprintType)
struct FItemTypes
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadOnly)
	FString Normal_Melee = "Normal_Melee";
	
	UPROPERTY(BlueprintReadOnly)
	FString Normal_Ranged = "Normal_Ranged";
};
