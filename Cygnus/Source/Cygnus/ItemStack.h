#pragma once

#include "CoreMinimal.h"
#include "CygnusError.h"
#include "ItemStruct.h"
#include "ItemStack.generated.h"

USTRUCT(BlueprintType)
struct FItemStack
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FName ItemName = FName("");

	TArray<FItemStruct> Items;

	ECygnusError Add(FItemStruct Item)
	{
		if (ItemName == FName(""))
		{
			ItemName = Item.Name;
		}
		else if (ItemName.IsEqual(Item.Name))
		{
			return ECygnusError::ItemMismatch;
		}
		
		Items.Add(Item);
		
		return ECygnusError::NoError;
	}

	ECygnusError Delete(FItemStruct Item)
	{
		// Items.Remove(Item);
	}

	ECygnusError Clear()
	{
		ItemName = FName("");
		
		Items.Empty();
	}
};
