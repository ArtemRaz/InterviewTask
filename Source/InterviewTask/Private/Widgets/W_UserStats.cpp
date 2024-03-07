// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/W_UserStats.h"

void UW_UserStats::SetUsername(FString Username)
{
	UsernameTextBlock->SetText(FText::FromString(Username));
}

void UW_UserStats::SetHealthPercent(const float NewHealthPercent)
{
	HealthProgressBar->SetPercent(NewHealthPercent);
	HealthProgressBar->SetFillColorAndOpacity(NewHealthPercent <=.5f ? LowHealthColor : HighHealthColor);
}
