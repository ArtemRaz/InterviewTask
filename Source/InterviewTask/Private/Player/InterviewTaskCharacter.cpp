// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/InterviewTaskCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Core/InterviewTaskGameInstance.h"
#include "Core/InterviewTaskGameMode.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameModeBase.h"
#include "Net/UnrealNetwork.h"
#include "Player/InterviewTaskPlayerState.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AInterviewTaskCharacter

AInterviewTaskCharacter::AInterviewTaskCharacter()
{
	SetCanBeDamaged(true);
	bReplicates = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	
	StatsWidgetComponent = CreateDefaultSubobject<UFacingWidgetComponent>(TEXT("StatsWidgetComponent"));
	StatsWidgetComponent->SetupAttachment(GetMesh());
	StatsWidgetComponent->SetRelativeLocation(FVector(0,0,GetCapsuleComponent()->GetScaledCapsuleHalfHeight()*2.5));
	StatsWidgetComponent->SetDrawSize(FVector2D(1500, 100));
	StatsWidgetComponent->bOwnerNoSee = true;
	
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SetRelativeLocation(FVector(0,0,GetCapsuleComponent()->GetScaledCapsuleHalfHeight()*2));

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AInterviewTaskCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
	StatsWidget = Cast<UW_UserStats>(StatsWidgetComponent->GetWidget());
	if (IsValid(StatsWidget))
	{
		StatsWidget->SetHealthPercent(Health/MaxHealth);
	}
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(LookMappingContext, 0);
			Subsystem->AddMappingContext(MovementMappingContext, 0);
			Subsystem->AddMappingContext(WeaponMappingContext, 0);
		}
	}

}

void AInterviewTaskCharacter::PostInitProperties()
{
	Super::PostInitProperties();

	if (GetNetMode()==NM_DedicatedServer && IsValid(WeaponClass))
	{
		Weapon = Cast<AWeapon>(GetWorld()->SpawnActor(WeaponClass));
		Weapon->SetOwner(this);
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), FName(TEXT("GripPoint")));
	}

	StatsWidgetComponent->SetWidgetClass(StatsWidgetClass);
}

float AInterviewTaskCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
                                          AController* EventInstigator, AActor* DamageCauser)
{
	const float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Health-=Damage;
	if (Health<=0)
	{
		if (UWorld* World = GetWorld())
		{
			if (AInterviewTaskGameMode* GameMode = Cast<AInterviewTaskGameMode>(World->GetAuthGameMode()))
			{
				GameMode->StartRespawn(GetController());
			}
		}
		Die();
	}
	return Damage;
}

void AInterviewTaskCharacter::Destroyed()
{
	Super::Destroyed();
	if (IsValid(Weapon))
	{
		Weapon->Destroy();
	}
}

void AInterviewTaskCharacter::StartFire()
{
	if (IsValid(Weapon))
	{
		Weapon->StartFire();
	}
}

void AInterviewTaskCharacter::FinishFire()
{
	if (IsValid(Weapon))
	{
		Weapon->FinishFire();
	}
}

void AInterviewTaskCharacter::Reload()
{
	if (IsValid(Weapon))
	{
		Weapon->StartReload();
	}
}

void AInterviewTaskCharacter::Die()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(MovementMappingContext);
			Subsystem->RemoveMappingContext(WeaponMappingContext);
		}
	}
	
	GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static FName MeshCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetCollisionProfileName(MeshCollisionProfileName);
}


void AInterviewTaskCharacter::OnRep_Health()
{
	StatsWidget->SetHealthPercent(Health/MaxHealth);
	if (Health<=0)
	{
		Die();
	}
}

void AInterviewTaskCharacter::SetUsername(const FString& NewUsername)
{
	if (IsValid(StatsWidget))
	{
		StatsWidget->SetUsername(NewUsername);
	}
}

void AInterviewTaskCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AInterviewTaskCharacter, Health);
	DOREPLIFETIME(AInterviewTaskCharacter, Weapon);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AInterviewTaskCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AInterviewTaskCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AInterviewTaskCharacter::Look);

		// Weapon
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AInterviewTaskCharacter::StartFire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AInterviewTaskCharacter::FinishFire);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AInterviewTaskCharacter::Reload);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AInterviewTaskCharacter::OnPlayerStateChanged(APlayerState* NewPlayerState, APlayerState* OldPlayerState)
{
	Super::OnPlayerStateChanged(NewPlayerState, OldPlayerState);

	if (AInterviewTaskPlayerState* TaskPlayerState = Cast<AInterviewTaskPlayerState>(NewPlayerState))
	{
		TaskPlayerState->OnUsernameChanged.AddDynamic(this, &AInterviewTaskCharacter::SetUsername);
		SetUsername(TaskPlayerState->GetUsername());
	}
}

void AInterviewTaskCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AInterviewTaskCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}