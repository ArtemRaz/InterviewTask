// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ConnectMenu.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UConnectMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	ConnectButton->OnClicked.AddDynamic(this, &UConnectMenu::OnClickedConnect);
	QuitButton->OnClicked.AddDynamic(this, &UConnectMenu::OnClickedQuit);
	
}

void UConnectMenu::OnClickedConnect()
{
	if (UsernameTextBox->GetText().IsEmpty())
	{
		ShowWarning("You must provide a username before connecting");
		return;
	}
	UGameplayStatics::OpenLevel(this, TEXT("127.0.0.1:7777"), true);
}

void UConnectMenu::OnClickedQuit()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	const TEnumAsByte QuitPreference = EQuitPreference::Quit;

	UKismetSystemLibrary::QuitGame(this, PlayerController, QuitPreference, true);
}

void UConnectMenu::ShowWarning(const FString& Message, const float Time)
{
	WarningTextBlock->SetText(FText::FromString(Message));
	WarningTextBlock->SetVisibility(ESlateVisibility::Visible);

	GetWorld()->GetTimerManager().SetTimer(WarningTimerHandle, this, &UConnectMenu::HideWarning,
	                                       Time, false);
}

void UConnectMenu::HideWarning()
{
	WarningTextBlock->SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetTimerManager().ClearTimer(WarningTimerHandle);
}
