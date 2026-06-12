// Copyright Epic Games, Inc. All Rights Reserved.

#include "AshfallUEGameMode.h"
#include "AshfallUECharacter.h"
#include "UObject/ConstructorHelpers.h"

AAshfallUEGameMode::AAshfallUEGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
