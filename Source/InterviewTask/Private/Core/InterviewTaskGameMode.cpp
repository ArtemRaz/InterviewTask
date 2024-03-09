// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/InterviewTaskGameMode.h"

#include "TimerManager.h"
#include "GameFramework/Character.h"


void AInterviewTaskGameMode::StartRespawn(AController* Controller)
{
	
	FTimerHandle TimerHandle;
	const FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &AInterviewTaskGameMode::FinishRespawn,
	                                                                     Controller);

	GetWorldTimerManager().SetTimer(TimerHandle, RespawnDelegate, DelayBeforeRestart, false);
}

void AInterviewTaskGameMode::FinishRespawn(AController* Controller)
{
	if (APawn* Pawn = Controller->GetPawn())
	{
		Pawn->Destroy();
	}
	Controller->UnPossess();
	RestartPlayer(Controller);
}
