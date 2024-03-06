// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "ConnectMenu.generated.h"

/**
 * 
 */
UCLASS()
class INTERVIEWTASK_API UConnectMenu : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(meta = (BindWidget))
	UButton* ConnectButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;
	
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* UsernameTextBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WarningTextBlock;

	UPROPERTY()
	FTimerHandle WarningTimerHandle;
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickedConnect();
	
	UFUNCTION()
	void OnClickedQuit();

	UFUNCTION()
	void ShowWarning(const FString& Message, float Time=5.f);

	UFUNCTION()
	void HideWarning();

};