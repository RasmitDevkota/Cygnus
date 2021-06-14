#include "Inventory.h"

#include "CygnusGameInstance.h"

UInventory::UInventory()
{
	UE_LOG(LogTemp, Warning, TEXT("Inventory instantiated"));
}

ECygnusError UInventory::Add(FItemStruct Item)
{
	return AddToHotbar(Item, true);
}

ECygnusError UInventory::AddToHotbar(FItemStruct Item, bool insist)
{
	if (Hotbar.Contains(Item.Name) && Item.CanStack)
	{
		FItemStack ItemStack = *Hotbar.Find(Item.Name);

		ItemStack.Add(Item);
	}
		
	if (Hotbar.Num() == 10)
	{
		if (insist)
		{
			return AddToCloudSack(Item);
		}
			
		return ECygnusError::HotbarFull;
	}

	return ECygnusError::NoError;
}

ECygnusError UInventory::AddToCloudSack(FItemStruct Item)
{
	if (RiftSack.Num() == RiftSackCapacity)
	{
		return ECygnusError::RiftSackFull;
	}
		
	return ECygnusError::NoError;
}

void UInventory::SetInventory(TArray<FDocument> Documents)
{
	UE_LOG(LogTemp, Warning, TEXT("Setting inventory"));
		
	for (FDocument& ItemDocument: Documents)
	{
		FItemStruct Item = FItemStruct(ItemDocument.Fields);
			
		FItemStack ItemStack = FItemStack();
		ItemStack.Add(Item);
			
		if (Item.Position < 10)
		{
			Hotbar.Add(Item.Name, ItemStack);

			UE_LOG(LogTemp, Warning, TEXT("Item '%s' added to Hotbar with new size %d"), *Item.Name.ToString(), Hotbar.Num());
		}
		else if (Item.Position < RiftSackCapacity + 10)
		{
			RiftSack.Add(Item.Name, ItemStack);

			UE_LOG(LogTemp, Warning, TEXT("Item '%s' added to RiftSack with new size %d"), *Item.Name.ToString(), RiftSack.Num());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("User data contains illegal object count (probably a security vulnerability or duplicate data)"));
		}
	}

	Fetched = true;

	UE_LOG(LogTemp, Warning, TEXT("Finished setting inventory"));

	UE_LOG(LogTemp, Warning, TEXT("Local Hotbar Size: %d"), Hotbar.Num());
	
	OnInventorySet.Broadcast(true);
}

ECygnusError UInventory::FetchInventory(UFirebase* FirebaseObject)
{
	FirebaseObject->OnListDocumentsRequestComplete.AddDynamic(this, &UInventory::SetInventory);
		
	FirebaseObject->FirebaseList("users/" + FirebaseObject->userId + "/Inventory");
		
	return ECygnusError::NoError;
}