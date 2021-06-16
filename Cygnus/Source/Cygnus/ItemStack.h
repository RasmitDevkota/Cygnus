#pragma once

#include "CoreMinimal.h"
#include "ItemObject.h"
#include "CygnusError.h"
#include "ItemStack.generated.h"

USTRUCT(BlueprintType)
struct FItemStack
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FName ItemName = FName("");

	UPROPERTY(BlueprintReadWrite)
	TArray<UItemObject*> Items;

	FItemStack()
	{
		
	}

	FItemStack(UItemObject* Item)
	{
		ItemName = Item->Name;

		Items.Add(Item);
	}

	ECygnusError Add(UItemObject* Item)
	{
		if (ItemName == FName(""))
		{
			ItemName = Item->Name;
		}
		else if (ItemName.IsEqual(Item->Name))
		{
			return ECygnusError::ItemMismatch;
		}
		
		Items.Add(Item);
		
		return ECygnusError::NoError;
	}

	ECygnusError Delete()
	{
		// TODO("Fix this so it actually removes the correct item and instead of assuming all items are equal (hint: use pointers/references or item IDs)")
		
		if (Items.Num() == 0)
		{
			return ECygnusError::NoError;
		}

		Items.Pop();
		
		return ECygnusError::NoError;
	}

	ECygnusError Clear()
	{
		ItemName = FName("");
		
		Items.Empty();

		return ECygnusError::NoError;
	}
};
