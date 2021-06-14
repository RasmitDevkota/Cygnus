#pragma once

#include "CoreMinimal.h"
#include "Document.generated.h"

USTRUCT(BlueprintType)
struct FDocument
{
	GENERATED_USTRUCT_BODY();

	FString Name;

	FJsonObject Fields;

	FJsonObject Data;

	FDocument()
	{
		Name = "";
		Fields = FJsonObject();
		
		Data = FJsonObject();
	}

	FDocument(FJsonObject _Data)
	{
		Data = _Data;

		Name = Data.GetStringField("name");

		Fields = *Data.GetObjectField("fields");
	}
};
