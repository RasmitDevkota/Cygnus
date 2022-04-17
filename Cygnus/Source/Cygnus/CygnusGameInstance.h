#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Firebase.h"
#include "Inventory.h"
#include "WebSocket.h"

#include "CygnusGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryInstantiatedDelegate, bool, result);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryToggledDelegate, bool, result);

UCLASS()
class CYGNUS_API UCygnusGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UCygnusGameInstance();

	int InstanceIdentifier = 0;

	bool DiscordActivitySet = false;
	
	UPROPERTY(BlueprintReadWrite)
    UFirebase* FirebaseObject;

	UPROPERTY(BlueprintReadWrite)
	UWebSocket* WebSocket;

    UPROPERTY(BlueprintReadWrite)
	UInventory* Inventory;

	UFUNCTION(BlueprintCallable)
	TArray<FName> GetHotbar();
	
	UFUNCTION(BlueprintCallable)
	TArray<FName> GetRiftSack();

	UFUNCTION(BlueprintCallable)
	void ToggleInventory();

	UPROPERTY(BlueprintAssignable)
	FInventoryInstantiatedDelegate OnInventoryInstantiated;

	UPROPERTY(BlueprintAssignable)
	FInventoryToggledDelegate OnInventoryToggled;
};
