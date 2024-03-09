// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InterviewTaskGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDiedSignature, ACharacter*, Character);

UCLASS(minimalapi)
class AInterviewTaskGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=GamePlay,meta = (AllowPrivateAccess = "true"))
	int32 DelayBeforeRestart=3;
	
	UFUNCTION()
	void FinishRespawn(AController* Controller);
	
public:
	UFUNCTION()
	void StartRespawn(AController* Controller);

};



