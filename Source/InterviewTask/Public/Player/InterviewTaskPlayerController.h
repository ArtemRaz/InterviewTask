// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "InterviewTaskPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AInterviewTaskPlayerController : public APlayerController
{
	GENERATED_BODY()
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void Quit();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* QuitAction;
};
