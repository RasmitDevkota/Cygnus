#include "ItemObject.h"

#include "CygnusGameInstance.h"

UItemObject::UItemObject()
{

}

UItemObject::UItemObject(FJsonObject Fields)
{
	TArray<FString> FieldNames;

	Fields.Values.GetKeys(FieldNames);

	Name = FName(Fields.GetObjectField("name")->GetStringField("stringValue"));
	Description = Fields.GetObjectField("description")->GetStringField("stringValue");
	Class = FName(Fields.GetObjectField("class")->GetStringField("stringValue"));
	Type = FName(Fields.GetObjectField("type")->GetStringField("stringValue"));

	CanStack = Fields.GetObjectField("canStack")->GetStringField("booleanValue").ToBool();

	Position = FCString::Atoi(*Fields.GetObjectField("position")->GetStringField("integerValue"));

	// New Idea: ...why? Ignore for now.
	// Old Idea: TODO("Use enum (or similar solution) to map stats to TArray<int Order, FName DisplayName(, function FormatValue)>")

	if (Fields.GetObjectField("stats")->GetObjectField("mapValue")->HasField("fields"))
	{
		for (auto Stat: Fields.GetObjectField("stats")->GetObjectField("mapValue")->GetObjectField("fields")->Values)
		{
			FItemStat* StatValue = new FItemStat(*Stat.Value->AsObject().Get());

			StatValue->Name = FName(Stat.Key);

			ItemStats.Add(FName(Stat.Key), *StatValue);
		}
	}

	FirebaseObject = GetWorld()->GetGameInstance<UCygnusGameInstance>()->FirebaseObject;
}

// UItemObject::~UItemObject()
// {
// 	if (NeedsPush)
// 	{
// 		FirebaseObject->FirebaseUpdate("users/" + FirebaseObject->userId + "/Inventory/" + Name.ToString(), "");
// 	}
// }

void UItemObject::Set(FJsonObject Fields)
{
	TArray<FString> FieldNames;
	
	Fields.Values.GetKeys(FieldNames);
	
	Name = FName(Fields.GetObjectField("name")->GetStringField("stringValue"));
	Description = Fields.GetObjectField("description")->GetStringField("stringValue");
	Class = FName(Fields.GetObjectField("class")->GetStringField("stringValue"));
	Type = FName(Fields.GetObjectField("type")->GetStringField("stringValue"));

	CanStack = Fields.GetObjectField("canStack")->GetStringField("booleanValue").ToBool();

	Position = FCString::Atoi(*Fields.GetObjectField("position")->GetStringField("integerValue"));

	if (Fields.GetObjectField("stats")->GetObjectField("mapValue")->HasField("fields"))
	{
		for (auto Stat: Fields.GetObjectField("stats")->GetObjectField("mapValue")->GetObjectField("fields")->Values)
		{
			FItemStat* StatValue = new FItemStat(*Stat.Value->AsObject().Get());

			StatValue->Name = FName(Stat.Key);

			ItemStats.Add(FName(Stat.Key), *StatValue);
		}
	}
}

TArray<FString> UItemObject::GetDetailsString()
{
	TArray<FString> Details;
	
	TArray<FString> StatNames;

	for (auto& Stat: ItemStats)
	{
		StatNames.Add(Stat.Key.ToString());
	}

	if (MasterStatList.Num() == 0)
	{
		MasterStatList.Append({
			"Physical Damage",
			"Mystic Damage",
			"Damage Shield",
			"Strength",
			"Energy",
			"Stamina",
			"Mobility",
			"Movement Speed",
			"Jump Height",
			"Fly Time",
			"Stealth",
			"Awareness",
			"Longevity"
		});
	}

	StatNames.Sort([&](const FString& A, const FString& B)
	{
		return MasterStatList.Find(A) < MasterStatList.Find(B);
	});

	for (FString& StatName: StatNames)
	{
		FItemStat* ItemStat = ItemStats.Find(FName(StatName));
		Details.Add(ItemStat->Name.ToString() + ": " + ItemStat->ToString());
	}

	Details.Insert(Description, 0);
	Details.Insert("\n", 1);

	return Details;
}
