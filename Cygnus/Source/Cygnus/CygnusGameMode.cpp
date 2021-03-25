// Copyright Epic Games, Inc. All Rights Reserved.

#include "CygnusGameMode.h"
#include "CygnusCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACygnusGameMode::ACygnusGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
