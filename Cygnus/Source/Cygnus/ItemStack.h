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
	int StackPosition = 10;

	UPROPERTY(BlueprintReadWrite)
	TArray<UItemObject*> Items;

	FItemStack()
	{
		
	}

	FItemStack(UItemObject* Item)
	{
		ItemName = Item->Name;

		StackPosition = Item->Position;

		Items.Add(Item);
	}

	ECygnusError Push(UItemObject* Item)
	{
		// TODO("When adding ItemObject to ItemStack, make sure the ItemObject position and ItemStack position are the same")
		
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

	ECygnusError Pop()
	{
		// TODO("Fix this so it actually removes the correct item and instead of assuming all items are equal (hint: use pointers/references or item IDs)")
		// --> Unnecessary if objects that can be unique are all !canStack

		// TODO("Delete ItemStack instance or save for reuse? If both, how can we check?")
		// --> Saving for reuse would be pointless for Rift Sack since everything is stored and RiftSack is refilled, should be deleted
		// --> Saving for Hotbar would be useful since player is likely to keep all/most of Hotbar filled at all times, no point reinstantiating
		// --> To check if doing both, guarantee of external management would be better than an internal variable
		
		if (Items.Num() == 0)
		{
			return ECygnusError::NoError;
		}

		Items.Pop();

		if (Items.Num() == 0)
		{
			ItemName = FName("");

			StackPosition = 10;
		}
		
		return ECygnusError::NoError;
	}

	ECygnusError Clear()
	{
		ItemName = FName("");

		StackPosition = 10;
		
		Items.Empty();

		return ECygnusError::NoError;
	}
};
