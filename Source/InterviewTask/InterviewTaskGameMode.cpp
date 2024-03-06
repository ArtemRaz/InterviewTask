// Copyright Epic Games, Inc. All Rights Reserved.

#include "InterviewTaskGameMode.h"
#include "InterviewTaskCharacter.h"
#include "UObject/ConstructorHelpers.h"

AInterviewTaskGameMode::AInterviewTaskGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
