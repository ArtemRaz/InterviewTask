// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FacingWidgetComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


UFacingWidgetComponent::UFacingWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFacingWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsValid(CameraManager))
	{
		const FVector WorldLocation = GetComponentToWorld().GetLocation();
		FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(WorldLocation, CameraManager->GetCameraLocation());
		// Rotator.Pitch = 0;
		// Rotator.Yaw -= 90;
		// Rotator.Roll = -Rotator.Pitch+90; 
		SetWorldRotation(Rotator);
	}
	else
	{
		CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	}
	
}
