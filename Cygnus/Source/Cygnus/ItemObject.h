#pragma once

#include "CoreMinimal.h"

#include "Firebase.h"
#include "ItemStat.h"
#include "UObject/Object.h"
#include "ItemObject.generated.h"

UCLASS()
class CYGNUS_API UItemObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName Name = FName("");

	UPROPERTY()
	FString Description = "";

	UPROPERTY()
	FName Class = FName("");

	UPROPERTY()
	FName Type = FName("");

	UPROPERTY()
	bool CanStack = false;

	UPROPERTY()
	int Position = 0;

	TMap<FName, FItemStat> ItemStats = TMap<FName, FItemStat>();

	TArray<FString> MasterStatList = TArray<FString>();

	bool NeedsPush = false;

	UFirebase* FirebaseObject;

	UItemObject();

	UItemObject(FJsonObject Fields);

	~UItemObject();

	void Set(FJsonObject Fields);

	UFUNCTION(BlueprintCallable)
	TArray<FString> GetDetailsString();
};
