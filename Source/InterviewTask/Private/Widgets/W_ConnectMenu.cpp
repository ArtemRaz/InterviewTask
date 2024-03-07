// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/W_ConnectMenu.h"
#include "TimerManager.h"
#include "Core/InterviewTaskGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UW_ConnectMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	ConnectButton->OnClicked.AddDynamic(this, &UW_ConnectMenu::OnClickedConnect);
	QuitButton->OnClicked.AddDynamic(this, &UW_ConnectMenu::OnClickedQuit);
	
}

void UW_ConnectMenu::OnClickedConnect()
{
	const FText Username = UsernameTextBox->GetText();
	if (Username.IsEmpty())
	{
		ShowWarning("You must provide a username before connecting");
		return;
	}
	if (UInterviewTaskGameInstance* GameInstance = Cast<UInterviewTaskGameInstance>(GetGameInstance()))
	{
		GameInstance->Username = Username.ToString();
	}
	UGameplayStatics::OpenLevel(this, TEXT("127.0.0.1:7777"), true);
}

void UW_ConnectMenu::OnClickedQuit()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	const TEnumAsByte QuitPreference = EQuitPreference::Quit;

	UKismetSystemLibrary::QuitGame(this, PlayerController, QuitPreference, true);
}

void UW_ConnectMenu::ShowWarning(const FString& Message, const float Time)
{
	WarningTextBlock->SetText(FText::FromString(Message));
	WarningTextBlock->SetVisibility(ESlateVisibility::Visible);

	GetWorld()->GetTimerManager().SetTimer(WarningTimerHandle, this, &UW_ConnectMenu::HideWarning,
	                                       Time, false);
}

void UW_ConnectMenu::HideWarning()
{
	WarningTextBlock->SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetTimerManager().ClearTimer(WarningTimerHandle);
}
