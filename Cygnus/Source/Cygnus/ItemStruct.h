#pragma once

#include "CoreMinimal.h"
#include "ItemStats.h"
#include "JsonObjectConverter.h"
#include "ItemStruct.generated.h"

USTRUCT(BlueprintType)
struct FItemStruct
{
	GENERATED_USTRUCT_BODY();
	
	UPROPERTY()
	FName Name = FName("");

	UPROPERTY()
	FName Class = FName("");

	UPROPERTY()
	FName Type = FName("");

	UPROPERTY()
	bool CanStack = false;

	UPROPERTY()
	int Position = 0;

	UPROPERTY()
	FItemStats Stats = FItemStats();

	FItemStruct()
	{
		
	}

	FItemStruct(FJsonObject Fields)
	{
		TArray<FString> FieldNames;
		
		Fields.Values.GetKeys(FieldNames);

		for (FString& FieldName: FieldNames)
		{
			UE_LOG(LogTemp, Warning, TEXT("FieldName: %s"), *FieldName);
		}
		
		Name = FName(Fields.GetObjectField("name")->GetStringField("stringValue"));
		Class = FName(Fields.GetObjectField("class")->GetStringField("stringValue"));
		Type = FName(Fields.GetObjectField("type")->GetStringField("stringValue"));
		
		CanStack = Fields.GetObjectField("canStack")->GetStringField("booleanValue").ToBool();

		FJsonObjectConverter::JsonObjectToUStruct(Fields.GetObjectField("stats")->GetObjectField("mapValue")->GetObjectField("fields").ToSharedRef(), &Stats);
	}
};
