#include "CygnusGameInstance.h"

UCygnusGameInstance::UCygnusGameInstance()
{
	InstanceIdentifier = rand();
	
	FirebaseObject = NewObject<UFirebase>();

	WebSocket = NewObject<UWebSocket>();
	
	Inventory = NewObject<UInventory>();

	UE_LOG(LogTemp, Warning, TEXT("Game Instance %d constructed"), InstanceIdentifier);
}

TArray<FName> UCygnusGameInstance::GetHotbar()
{
	TArray<FName> HotbarArray = TArray<FName>();
	
	if (Inventory == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory does not exist"));
		
		return HotbarArray;
	}
	
	if (!Inventory->Fetched)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory has not been fetched"));
		
		Inventory->FetchInventory(FirebaseObject);

		return HotbarArray;
	}
	
	TArray<FName> Hotbar;
	Inventory->Hotbar.GetKeys(Hotbar);

	TArray<int> Positions;

	for (auto ItemEntry: Inventory->Hotbar)
	{
		Positions.Add(ItemEntry.Value.StackPosition);
	}

	HotbarArray.Init(FName("NoItem"), 10);

	for (int i = 0; i < Positions.Num(); i++)
	{
		int Position = Positions[i];
		HotbarArray[Position] = Hotbar[i];
	}

	return HotbarArray;
}

TArray<FName> UCygnusGameInstance::GetRiftSack()
{
	TArray<FName> RiftSackArray = TArray<FName>();
	
	if (Inventory == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory does not exist"));
		
		return RiftSackArray;
	}
	
	if (!Inventory->Fetched)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory has not been fetched"));
		
		Inventory->FetchInventory(FirebaseObject);

		return RiftSackArray;
	}

	TArray<FName> RiftSack;

	Inventory->RiftSack.GetKeys(RiftSack);

	// TODO("Should Rift Sack be filled to capacity in case user gets more items or filled dynamically?")
	// --> "Filled to capacity" is actually dynamic anyways since capacity can increase, might as well fill dynamically?
	// --> If filled dynamically, space is saved but performance is decreased, does it matter though?
	// --> If filled to capacity, Rift Sack must be filled with empty items - is this okay or should we find a representation of an empty item (i.e. a single shared EmptyItemStack)?
	// --> If filled dynamically, we have to remove dynamically, more performance costs
	
	for (int i = 0; i < RiftSack.Num(); i++)
	{
		RiftSackArray.Add(RiftSack[i]);
	}

	return RiftSackArray;
}

void UCygnusGameInstance::ToggleInventory()
{
	UE_LOG(LogTemp, Warning, TEXT("Called broadcast Inventory Toggle"));
	
	if (Inventory->Fetched)
	{
		OnInventoryToggled.Broadcast(true);

		UE_LOG(LogTemp, Warning, TEXT("Broadcasted Inventory Toggle"));
	}
}
