#include "ItemObject.h"

UItemObject::UItemObject()
{
	
}

UItemObject::UItemObject(FJsonObject Fields)
{
	TArray<FString> FieldNames;
		
	Fields.Values.GetKeys(FieldNames);

	for (FString& FieldName: FieldNames)
	{
		UE_LOG(LogTemp, Warning, TEXT("FieldName: %s"), *FieldName);
	}
		
	Name = FName(Fields.GetObjectField("name")->GetStringField("stringValue"));
	Description = Fields.GetObjectField("name")->GetStringField("stringValue");
	Class = FName(Fields.GetObjectField("class")->GetStringField("stringValue"));
	Type = FName(Fields.GetObjectField("type")->GetStringField("stringValue"));
		
	CanStack = Fields.GetObjectField("canStack")->GetStringField("booleanValue").ToBool();

	for (auto Stat: Fields.GetObjectField("stats")->GetObjectField("mapValue")->GetObjectField("fields")->Values)
	{
		FItemStat* StatValue = new FItemStat(*Stat.Value->AsObject().Get());
			
		ItemStats.Add(FName(Stat.Key), *StatValue);
	}
}

void UItemObject::Set(FJsonObject Fields)
{
	TArray<FString> FieldNames;
		
	Fields.Values.GetKeys(FieldNames);

	for (FString& FieldName: FieldNames)
	{
		UE_LOG(LogTemp, Warning, TEXT("FieldName: %s"), *FieldName);
	}
		
	Name = FName(Fields.GetObjectField("name")->GetStringField("stringValue"));
	Description = Fields.GetObjectField("name")->GetStringField("stringValue");
	Class = FName(Fields.GetObjectField("class")->GetStringField("stringValue"));
	Type = FName(Fields.GetObjectField("type")->GetStringField("stringValue"));
		
	CanStack = Fields.GetObjectField("canStack")->GetStringField("booleanValue").ToBool();

	for (auto Stat: Fields.GetObjectField("stats")->GetObjectField("mapValue")->GetObjectField("fields")->Values)
	{
		FItemStat* StatValue = new FItemStat(*Stat.Value->AsObject().Get());
			
		ItemStats.Add(FName(Stat.Key), *StatValue);
	}
}

TArray<FString> UItemObject::GetDetailsString()
{
	TArray<FString> Details;

	Details.Add(Description);

	Details.Add("\n");
		
	for (auto Stat: ItemStats)
	{
		Details.Add("\n");
		Details.Add(Stat.Key.ToString() + ": " + Stat.Value.ToString());
	}

	return Details;
}