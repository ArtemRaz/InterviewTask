// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "FacingWidgetComponent.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, ClassGroup="UserInterface", meta=(BlueprintSpawnableComponent))
class INTERVIEWTASK_API UFacingWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	UFacingWidgetComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	APlayerCameraManager* CameraManager;
};
