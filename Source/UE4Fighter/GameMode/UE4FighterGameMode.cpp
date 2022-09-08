// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE4FighterGameMode.h"
#include "../Character/UE4FighterCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUE4FighterGameMode::AUE4FighterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
