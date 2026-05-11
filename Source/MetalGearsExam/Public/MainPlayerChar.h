// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
class USpringArmComponent;
class UCameraComponent;

//gives mouse values
#include "InputActionValue.h"
class UInputMappingContext;
class UInputAction;

//Has to be last
#include "MainPlayerChar.generated.h"

UCLASS()
class METALGEARSEXAM_API AMainPlayerChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayerChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//DamageEvent Sets up function Gameover
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent 
		const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Game Flow")
	void GameOverFunc();
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//MainVariable and ComponentList
	//Components
	//playerEquips
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UPlayerEquipComponent* PlayerEquipment;
	
	//Springarm
	UPROPERTY(VisibleAnywhere, BlueprintreadOnly, Category = "Components")
	USpringArmComponent* CSpringArm;
	//Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* PlayerCamera;
protected:
	//LadderGroup
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ladder Group")
	bool bIsClimbing;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ladder Group")
	float ClimbSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ladder Group")
	bool CurrentLadderCollideTop;
	
	//C++ doesnt do bp calls apparently, so calling actor.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ladder Group")
	class AActor* CurrentLadder;
	
	//Stealthdetection Is in its own component class
	//HuDGroup (will be handled by the blueprint)
	//Inventory will be in its own component PlayerEquip
	//Playerhealth has been simplfied to just react to any damage
	
	//XrayCrouchPause Variables
	UPROPERTY(editAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bKeyHoldSetting;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "States")
	bool bIsXrayActive;
	
	//Movement variables
	// Mapping context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	//WaSddaaa movement
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	// MouseLook
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;
	
	//Crouching
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* CrouchAction;
	
	//Winanimation
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Visuals")
	void BP_PlayVictoryCelebration();
	
	// Keycard refreshingui style
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Inventory")
	void RefreshKeycardUI();

	//Xray
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* XRayAction;
	
	//Pausemenu
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* PauseAction;
	
	//Shoot
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ShootAction;
	
	//Knifetoss
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* KnifeTossAction;
	//Duuuuckieeee
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* DuckieThrowAction;
	
	//Interact
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractAction;
	
	// Lets enemies update detection overlay
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Visuals")
	void UpdateDetectionOverlay(bool bIsDetected);
	
	//Allows exitladder custom event from bp
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Movement")
	void ExitLadder();
	
	//Ladder Y input for custom event check
	UFUNCTION(BlueprintImplementableEvent, Category = "Movement")
	void ClimbLadderLogic(float YInput);
	
	// Functions for input movement
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	//Input functions wrappers
	void InputCrouch(const FInputActionValue& Value);
	void InputXRay(const FInputActionValue& Value);
	void InputPause(const FInputActionValue& Value);
	void InputShoot(const FInputActionValue& Value);
	void InputKnifeToss(const FInputActionValue& Value);
	void InputDuckie(const FInputActionValue& Value);
	void FireInteractionSphereTrace();
	
	//Fires rayline interact
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void BP_FireInteractionRayline();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void BP_OnCrouchStateChanged(bool bIsNowCrouching);

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void BP_OnXRayStateChanged(bool bIsNowActive);

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void BP_OnPauseMenuTriggered();
	
};
