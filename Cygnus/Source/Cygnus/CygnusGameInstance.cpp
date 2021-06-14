#include "CygnusGameInstance.h"

UCygnusGameInstance::UCygnusGameInstance()
{
	InstanceIdentifier = rand();
	
	FirebaseObject = NewObject<UFirebase>();
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

	UE_LOG(LogTemp, Warning, TEXT("CygnusGameInstance Hotbar Size: %d"), Hotbar.Num());

	for (int i = 0; i < 10; i++)
	{
		if (i < Hotbar.Num())
		{
			HotbarArray.Add(Hotbar[i]);
		}
		else
		{
			HotbarArray.Add(FName("TestItem"));

			UE_LOG(LogTemp, Warning, TEXT("Added filler TestItem"));
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Hotbar Item #%d: %s"), i, *FString(HotbarArray[i].ToString()));
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
	}
	
	TMap<FName, FItemStack> RiftSack = Inventory->RiftSack;

	for (int i = 0; i < Inventory->RiftSackCapacity; i++)
	{
		RiftSackArray.Add(FName("TestItem"));
	}

	return RiftSackArray;
}

void UCygnusGameInstance::OpenInventory()
{
	
}

void UCygnusGameInstance::CloseInventory()
{
	
}