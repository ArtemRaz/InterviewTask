// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "W_UserStats.generated.h"

/**
 * 
 */
UCLASS()
class INTERVIEWTASK_API UW_UserStats : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(meta = (BindWidget))
	UTextBlock* UsernameTextBlock;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;

public:
	UPROPERTY(BlueprintReadWrite)
	FLinearColor LowHealthColor = FLinearColor(1, 0, 0);

	UPROPERTY(BlueprintReadWrite)
	FLinearColor HighHealthColor = FLinearColor(0, 1, 0);

	UFUNCTION(BlueprintCallable)
	void SetUsername(FString Username);

	UFUNCTION(BlueprintCallable)
	void SetHealthPercent(float NewHealthPercent);
};
