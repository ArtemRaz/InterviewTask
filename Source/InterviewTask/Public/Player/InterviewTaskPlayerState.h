// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "InterviewTaskPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class INTERVIEWTASK_API AInterviewTaskPlayerState : public APlayerState
{
	GENERATED_BODY()
	
	UPROPERTY(ReplicatedUsing=OnRep_Username)
	FString Username;
protected:
	UFUNCTION()
	void OnRep_Username();
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUsernameChangedEvent, const FString&, NewUsername);

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(Server, Reliable)
	void SetUsername(const FString& NewUsername);

	UFUNCTION()
	FORCEINLINE FString GetUsername() const {return Username;}
	
	UPROPERTY()
	FOnUsernameChangedEvent OnUsernameChanged;
};
