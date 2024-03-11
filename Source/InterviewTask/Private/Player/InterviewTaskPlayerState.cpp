// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/InterviewTaskPlayerState.h"

#include "Core/InterviewTaskGameInstance.h"
#include "Net/UnrealNetwork.h"

void AInterviewTaskPlayerState::OnRep_Username()
{
	OnUsernameChanged.Broadcast(Username);
}

void AInterviewTaskPlayerState::BeginPlay()
{
	Super::BeginPlay();

	const UInterviewTaskGameInstance* GameInstance = Cast<UInterviewTaskGameInstance>(GetGameInstance());
	if (GetLocalRole() == ROLE_SimulatedProxy && GetOwningController() && IsValid(GameInstance))
	{
		SetUsername(GameInstance->Username);
	}
}

void AInterviewTaskPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AInterviewTaskPlayerState, Username);
}

void AInterviewTaskPlayerState::SetUsername_Implementation(const FString& NewUsername)
{
	Username = NewUsername;
}
