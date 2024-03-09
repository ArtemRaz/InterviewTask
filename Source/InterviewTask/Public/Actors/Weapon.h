// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Weapon.generated.h"

UCLASS(Blueprintable, BlueprintType)
class INTERVIEWTASK_API AWeapon : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Sounds, meta = (AllowPrivateAccess = "true"))
	USoundBase* ReloadSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Sounds, meta = (AllowPrivateAccess = "true"))
	USoundBase* ShootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Sounds, meta = (AllowPrivateAccess = "true"))
	USoundBase* EmptyWeaponSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Sounds, meta = (AllowPrivateAccess = "true"))
	USoundBase* HitSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY()
	int32 MagazineCapacity = 30;

	UPROPERTY(Replicated)
	int32 BulletsRemain = MagazineCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float ReloadDelay = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float FireDelay = 0.1;

	UPROPERTY()
	bool bShootIsAvailable = true;

	UPROPERTY()
	bool bIsFire = false;

	UPROPERTY()
	FTimerHandle ReloadHandle;

	UPROPERTY()
	FTimerHandle FireHandle;

	UPROPERTY()
	ACharacter* Character;

protected:
	UFUNCTION(Server, Reliable)
	void ProcessShot();

	UFUNCTION()
	void SetCharacter(ACharacter* NewCharacter);

	UFUNCTION(NetMulticast, Unreliable)
	void SpawnSound(USoundBase* Sound);

	UFUNCTION(NetMulticast, Unreliable)
	void SpawnSoundAtLocation(USoundBase* Sound, const FVector Location);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void FinishReload();

	UFUNCTION()
	void FinishShoot();

public:
	// Sets default values for this actor's properties
	AWeapon();

	virtual void Destroyed() override;

	UFUNCTION(Server, Reliable)
	void StartFire();

	UFUNCTION(Server, Reliable)
	void FinishFire();


	UFUNCTION(Server, Reliable)
	void StartReload();

	virtual void SetOwner(AActor* NewOwner) override;
};
