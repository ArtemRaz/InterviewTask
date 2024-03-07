// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "InterviewTaskGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class INTERVIEWTASK_API UInterviewTaskGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FString Username;
};
