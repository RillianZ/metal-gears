// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerChar.h"

#include <rapidjson/document.h>

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerEquipComponent.h"
#include "Interact.h"

//main constructor of Player char
AMainPlayerChar::AMainPlayerChar()
{
	PrimaryActorTick.bCanEverTick = true;

	//we attach spring arm
	CSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CSpringArm"));
	CSpringArm->SetupAttachment(RootComponent);
	CSpringArm->bUsePawnControlRotation = true;
	CSpringArm->TargetArmLength = 0.0f;
	CSpringArm->bEnableCameraLag = true;
	CSpringArm->CameraLagSpeed = 20.0f;
	CSpringArm->SetRelativeLocation(FVector(15.0f, 2.0f, 70.0f));

	//and camera
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(CSpringArm, USpringArmComponent::SocketName);
	PlayerCamera->bUsePawnControlRotation = true;
	PlayerCamera->PostProcessBlendWeight = 0.0f;

	//copied some of the variables from the variable list in bp
	bIsClimbing = false;
	ClimbSpeed = 150.0f;
	CurrentLadder = nullptr;
	CurrentLadderCollideTop = false;

	PlayerEquipment = CreateDefaultSubobject<UPlayerEquipComponent>(TEXT("PlayerEquipmentComp"));
}

//on begin play it ads the UE input system adds mappingcontext (assigned in details)
void AMainPlayerChar::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}
//due to time restraints, kept event tick in BP
void AMainPlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//binds the different action buttons
void AMainPlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainPlayerChar::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainPlayerChar::Look);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AMainPlayerChar::InputCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AMainPlayerChar::InputCrouch);

		EnhancedInputComponent->BindAction(XRayAction, ETriggerEvent::Started, this, &AMainPlayerChar::InputXRay);
		EnhancedInputComponent->BindAction(XRayAction, ETriggerEvent::Completed, this, &AMainPlayerChar::InputXRay);

		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &AMainPlayerChar::InputPause);

		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AMainPlayerChar::InputShoot);
		EnhancedInputComponent->BindAction(KnifeTossAction, ETriggerEvent::Started, this, &AMainPlayerChar::InputKnifeToss);

		EnhancedInputComponent->BindAction(DuckieThrowAction, ETriggerEvent::Started, this, &AMainPlayerChar::InputDuckie);
		EnhancedInputComponent->BindAction(DuckieThrowAction, ETriggerEvent::Completed, this, &AMainPlayerChar::InputDuckie);
		
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AMainPlayerChar::FireInteractionSphereTrace);
	}
}

//on take damage event, runs gameoverfunc 
float AMainPlayerChar::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.0f)
	{
		GameOverFunc();
	}

	return ActualDamage;
}

//movement system, standard, but also adds climbladder Y movement
void AMainPlayerChar::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		if (bIsClimbing)
		{
			ClimbLadderLogic(MovementVector.Y);
		}
		else
		{
			AddMovementInput(GetActorForwardVector(), MovementVector.Y);
			AddMovementInput(GetActorRightVector(), MovementVector.X);
		}
	}
}

//looking
void AMainPlayerChar::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

//toggle crouch built in unreal function but custom crouch state changed swosh noises in bp
void AMainPlayerChar::InputCrouch(const FInputActionValue& Value)
{
	bool bIsPressingKey = Value.Get<bool>();

	if (bKeyHoldSetting)
	{
		if (bIsPressingKey) { Crouch(); BP_OnCrouchStateChanged(true); }
		else { UnCrouch(); BP_OnCrouchStateChanged(false); }
	}
	else
	{
		if (bIsPressingKey)
		{
			if (bIsCrouched) { UnCrouch(); BP_OnCrouchStateChanged(false); }
			else { Crouch(); BP_OnCrouchStateChanged(true); }
		}
	}
}

//pings bp to turn on xray, uses some timelines and such to fade in so kept that in bp
void AMainPlayerChar::InputXRay(const FInputActionValue& Value)
{
	bool bIsPressingKey = Value.Get<bool>();

	if (bKeyHoldSetting)
	{
		if (bIsPressingKey && !bIsXrayActive)
		{
			bIsXrayActive = true;
			BP_OnXRayStateChanged(true);
		}
		else if (!bIsPressingKey && bIsXrayActive)
		{
			bIsXrayActive = false;
			BP_OnXRayStateChanged(false);
		}
	}
	else
	{
		if (bIsPressingKey)
		{
			bIsXrayActive = !bIsXrayActive;
			BP_OnXRayStateChanged(bIsXrayActive);
		}
	}
}

//triggers pause
void AMainPlayerChar::InputPause(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		BP_OnPauseMenuTriggered();
	}
}

//tries to shot gun
void AMainPlayerChar::InputShoot(const FInputActionValue& Value)
{
	if (PlayerEquipment)
	{
		if (PlayerEquipment->CurrentAmmoSilencer > 0)
		{
			PlayerEquipment->SetEquipState(EEquipState::Pistol);
			PlayerEquipment->TryShootGun();
		}
		else
		{
			FireInteractionSphereTrace();
		}
	}
}

//tries to throw knife
void AMainPlayerChar::InputKnifeToss(const FInputActionValue& Value)
{
	if (PlayerEquipment)
	{
		if (PlayerEquipment->CurrentAmmoKnife > 0)
		{
			PlayerEquipment->SetEquipState(EEquipState::Knife);
			PlayerEquipment->TryThrowKnife();
		}
		else
		{
			FireInteractionSphereTrace();
		}
	}
}

// tries to throw duckie
void AMainPlayerChar::InputDuckie(const FInputActionValue& Value)
{
	bool bIsPressed = Value.Get<bool>();
	if (PlayerEquipment)
	{
		if (bIsPressed) PlayerEquipment->InputDuckieStarted();
		else PlayerEquipment->InputDuckieCompleted();
	}
}

void AMainPlayerChar::FireInteractionSphereTrace()
{
	FVector Start = PlayerCamera->GetComponentLocation();
	FVector End = Start + (PlayerCamera->GetForwardVector() * 200.f);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepSingleByChannel(
		Hit,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(30.f),
		Params
	);

	if (bHit)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor) return;

		if (HitActor->GetClass()->ImplementsInterface(UInteract::StaticClass()))
		{
			IInteract::Execute_Interact(HitActor, this);
		}
	}
}