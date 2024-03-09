// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapon.h"


#include "CollisionQueryParams.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DamageEvents.h"
#include "Engine/HitResult.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundBase.h"


AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);
}

void AWeapon::Destroyed()
{
	Super::Destroyed();
	FTimerManager& TimerManager = GetWorldTimerManager();
	TimerManager.ClearTimer(FireHandle);
	TimerManager.ClearTimer(ReloadHandle);
}

void AWeapon::StartFire_Implementation()
{
	if (GetWorldTimerManager().IsTimerPending(ReloadHandle))
	{
		return;
	}
	bIsFire = true;
	ProcessShot();
}

void AWeapon::FinishFire_Implementation()
{
	bIsFire = false;
}

void AWeapon::ProcessShot_Implementation()
{
	if (BulletsRemain <= 0)
	{
		SpawnSound(EmptyWeaponSound);
		bIsFire = false;
		return;
	}
	if (!IsValid(Character) || !bShootIsAvailable)
	{
		return;
	}
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	if (!IsValid(PlayerController))
	{
		return;
	}
	APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
	if (!IsValid(CameraManager))
	{
		return;
	}

	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, Character);
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;

	FHitResult RV_Hit(ForceInit);

	FVector Start = CameraManager->GetCameraLocation();
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(CameraManager->GetCameraRotation()) * 10000;
	FVector End = Start + ForwardVector;

	GetWorld()->LineTraceSingleByChannel(
		RV_Hit,
		Start,
		End,
		ECC_Pawn,
		RV_TraceParams
	);

	if (RV_Hit.bBlockingHit)
	{
		SpawnSoundAtLocation(HitSound, RV_Hit.ImpactPoint);
		RV_Hit.GetActor()->TakeDamage(15, FPointDamageEvent{}, Character->Controller, Character);
	}
	SpawnSound(ShootSound);
	BulletsRemain -= 1;
	bShootIsAvailable = false;
	GetWorldTimerManager().SetTimer(FireHandle, this, &AWeapon::FinishShoot, FireDelay, false);
}

void AWeapon::FinishShoot()
{
	bShootIsAvailable = true;
	if (bIsFire)
	{
		ProcessShot();
	}
}

void AWeapon::StartReload_Implementation()
{
	// play reload animation
	if (GetWorldTimerManager().IsTimerPending(ReloadHandle))
	{
		GetWorldTimerManager().ClearTimer(FireHandle);
	}
	SpawnSound(ReloadSound);
	bIsFire = false;
	GetWorldTimerManager().SetTimer(ReloadHandle, this, &AWeapon::FinishReload, ReloadDelay);
}

void AWeapon::FinishReload()
{
	BulletsRemain = MagazineCapacity;
}

void AWeapon::SetCharacter(ACharacter* NewCharacter)
{
	Character = NewCharacter;
}

void AWeapon::SpawnSound_Implementation(USoundBase* Sound)
{
	if (IsValid(Sound))
	{
		UGameplayStatics::SpawnSoundAttached(Sound, RootComponent);
	}
}

void AWeapon::SpawnSoundAtLocation_Implementation(USoundBase* Sound, const FVector Location)
{
	if (IsValid(Sound))
	{
		UGameplayStatics::SpawnSoundAtLocation(this, Sound, Location);
	}
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeapon, BulletsRemain)
}

void AWeapon::PostInitProperties()
{
	Super::PostInitProperties();

	if (IsValid(Mesh))
	{
		MeshComponent->SetStaticMesh(Mesh);
		MeshComponent->SetCollisionProfileName("NoCollision");
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
	SetCharacter(Cast<ACharacter>(NewOwner));
}
