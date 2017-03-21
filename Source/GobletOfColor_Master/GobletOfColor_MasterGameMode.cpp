// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "GobletOfColor_Master.h"
#include "GobletOfColor_MasterGameMode.h"
#include "GobletOfColor_MasterCharacter.h"

AGobletOfColor_MasterGameMode::AGobletOfColor_MasterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
