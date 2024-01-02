// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShootingCodeGameMode.h"
#include "ShootingCodeGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AShootingCodeGameMode::AShootingCodeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
