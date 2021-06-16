#pragma once

#include "CoreMinimal.h"
#include "CygnusError.h"
#include "Firebase.h"
#include "ItemStack.h"
#include "UObject/Object.h"
#include "Inventory.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventorySetDelegate, bool, result);

UCLASS(BlueprintType)
class CYGNUS_API UInventory : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	bool Fetched = false;
	
	UPROPERTY(BlueprintReadWrite)
	TMap<FName, FItemStack> Hotbar = TMap<FName, FItemStack>();
	
	UPROPERTY(BlueprintReadWrite)
	TMap<FName, FItemStack> RiftSack = TMap<FName, FItemStack>();

	UPROPERTY(BlueprintReadWrite)
	int RiftSackCapacity = 10;

	UInventory();

	ECygnusError Add(UItemObject* Item);

	ECygnusError AddToHotbar(UItemObject* Item, bool insist = false);

	ECygnusError AddToRiftSack(UItemObject* Item);

	UFUNCTION(BlueprintCallable)
	void SetInventory(TArray<FDocument> Documents);

	UFUNCTION(BlueprintCallable)
	ECygnusError FetchInventory(UFirebase* FirebaseObject);

	UPROPERTY(BlueprintAssignable)
	FInventorySetDelegate OnInventorySet;
};
