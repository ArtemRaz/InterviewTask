// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/InterviewTaskPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/KismetSystemLibrary.h"

void AInterviewTaskPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(QuitAction, ETriggerEvent::Started, this, &AInterviewTaskPlayerController::Quit);
	}
}

void AInterviewTaskPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AInterviewTaskPlayerController::Quit()
{
	const TEnumAsByte QuitPreference = EQuitPreference::Quit;

	UKismetSystemLibrary::QuitGame(this, this, QuitPreference, true);
}
