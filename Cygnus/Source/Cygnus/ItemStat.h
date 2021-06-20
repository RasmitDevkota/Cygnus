#pragma once

#include "CoreMinimal.h"
#include "ItemStat.generated.h"

USTRUCT(BlueprintType)
struct FItemStat
{
	GENERATED_BODY();

	UPROPERTY()
	FName Name = FName("");

	UPROPERTY()
	FName Type = FName("");

	UPROPERTY()
	FString StringValue = "";
	
	UPROPERTY()
	float NumberValue = 0.0f;
	
	UPROPERTY()
	bool BooleanValue = true;

	FItemStat()
	{
		
	}

	FItemStat(FJsonObject StatObject)
	{
		TArray<FString> StatTypeKeys;
		StatObject.Values.GetKeys(StatTypeKeys);

		if (StatTypeKeys[0] == "stringValue")
		{
			Type = FName("String");
			
			StringValue = StatObject.GetStringField("stringValue");
		}
		else if (StatTypeKeys[0] == "integerValue")
		{
			Type = FName("Number");
			
			NumberValue = StatObject.GetNumberField("integerValue");
		}
		else if (StatTypeKeys[0] == "doubleValue")
		{
			Type = FName("Number");
			
			NumberValue = StatObject.GetNumberField("doubleValue");
		}
		else if (StatTypeKeys[0] == "booleanValue")
		{
			Type = FName("Boolean");
			
			BooleanValue = StatObject.GetBoolField("booleanValue");
		}
	}

	FItemStat(FString StatValue)
	{
		Type = FName("String");

		StringValue = StatValue;
	}

	FItemStat(float StatValue)
	{
		Type = FName("Number");

		NumberValue = StatValue;
	}

	FItemStat(bool StatValue)
	{
		Type = FName("Bool");

		BooleanValue = StatValue;
	}

	void Get(FString& destination)
	{
		destination = *StringValue;
	}

	void Get(float& destination)
	{
		destination = NumberValue;
	}

	void Get(bool& destination)
	{
		destination = BooleanValue;
	}

	void Set(FJsonObject StatObject)
	{
		TArray<FString> StatTypeKeys;
		StatObject.Values.GetKeys(StatTypeKeys);

		if (StatTypeKeys[0] == "stringValue")
		{
			Type = FName("String");
			
			StringValue = StatObject.GetStringField("stringValue");
		}
		else if (StatTypeKeys[0] == "integerValue")
		{
			Type = FName("Number");
			
			NumberValue = StatObject.GetNumberField("integerValue");
		}
		else if (StatTypeKeys[0] == "doubleValue")
		{
			Type = FName("Number");
			
			NumberValue = StatObject.GetNumberField("doubleValue");
		}
		else if (StatTypeKeys[0] == "booleanValue")
		{
			Type = FName("Boolean");
			
			BooleanValue = StatObject.GetBoolField("booleanValue");
		}
	}

	void Set(FString StatValue)
	{
		Type = FName("String");

		StringValue = StatValue;
	}

	void Set(float StatValue)
	{
		Type = FName("Double");

		NumberValue = StatValue;
	}

	void Set(bool StatValue)
	{
		Type = FName("Boolean");

		BooleanValue = StatValue;
	}

	FString ToString()
	{
		if (Type.IsEqual("String"))
		{
			return StringValue;
		}

		if (Type.IsEqual("Number"))
		{
			return FString::Printf(TEXT("%f"), NumberValue);
		}

		if (Type.IsEqual("Boolean"))
		{
			return BooleanValue ? "True" : "False";
		}

		return "";
	}
};
