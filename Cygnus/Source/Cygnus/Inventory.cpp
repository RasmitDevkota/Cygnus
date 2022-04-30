#include "Inventory.h"

#include "CygnusGameInstance.h"

UInventory::UInventory()
{
	UE_LOG(LogTemp, Warning, TEXT("Inventory instantiated"));
}

ECygnusError UInventory::Add(UItemObject* Item)
{
	return AddToHotbar(Item, true);
}

ECygnusError UInventory::AddToHotbar(UItemObject* Item, bool insist)
{
	if (Hotbar.Contains(Item->Name) && Item->CanStack)
	{
		Hotbar.Find(Item->Name)->Push(Item);
		
		return ECygnusError::NoError;
	}
	
	if (Hotbar.Num() == 10)
	{
		if (insist)
		{
			return AddToRiftSack(Item);
		}
			
		return ECygnusError::HotbarFull;
	}
	
	Hotbar.Add(Item->Name, FItemStack(Item));
	
	return ECygnusError::NoError;
}

ECygnusError UInventory::AddToRiftSack(UItemObject* Item)
{
	if (RiftSack.Num() >= RiftSackCapacity)
	{
		return ECygnusError::RiftSackFull;
	}
	
	if (RiftSack.Contains(Item->Name) && Item->CanStack)
	{
		RiftSack.Find(Item->Name)->Push(Item);
	}
	else
	{
		RiftSack.Add(Item->Name, FItemStack(Item));
	}
	
	return ECygnusError::NoError;
}

void UInventory::SetInventory(TArray<FDocument> Documents)
{
	UE_LOG(LogTemp, Warning, TEXT("Setting inventory"));

	for (int i = 0; i < Documents.Num(); i++)
	{
		FDocument& ItemDocument = Documents[i];
		
		UItemObject* Item = NewObject<UItemObject>();

		Item->Set(ItemDocument.Fields);
			
		FItemStack ItemStack = FItemStack(Item);

		// TODO("Implement and use Position (a.k.a. unique item identification) instead of index position (item order messes up)")
		// --> Sort when needed instead of in map, since 1. less computation needed and 2. easier using array index setter
		
		if (Item->Position < 10)
		{
			Hotbar.Add(Item->Name, ItemStack);
		}
		else if (i < RiftSackCapacity + 10)
		{
			RiftSack.Add(Item->Name, ItemStack);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("User data contains illegal object count (probably a security vulnerability or duplicate data)"));
		}
	}

	Fetched = true;

	UE_LOG(LogTemp, Warning, TEXT("Finished setting inventory"));
	
	OnInventorySet.Broadcast(true);
}

ECygnusError UInventory::FetchInventory(UFirebase* FirebaseObject)
{
	FirebaseObject->OnListDocumentsRequestComplete.AddDynamic(this, &UInventory::SetInventory);
		
	FirebaseObject->FirebaseList("users/" + FirebaseObject->userId + "/Inventory");
		
	return ECygnusError::NoError;
}