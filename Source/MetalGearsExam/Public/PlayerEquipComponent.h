// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerEquipComponent.generated.h"

//Declaring Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipEventSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FShootEffectsSignature, bool, bHitSomething, FVector, HitLocation, AActor*, HitActor);

//We remake the BP enum here since it controls equipstates and inventory stuff
UENUM(BlueprintType)
enum class EEquipState : uint8
{
	Unarmed UMETA(DisplayName="Unarmed"),
	Pistol UMETA(DisplayName="Pistol"),
	Knife UMETA(DisplayName="Knife"),
	NoVis UMETA(DisplayName="NoVision")
};

//sets up keycard enum for visuals
UENUM(BlueprintType)
enum class EKeycardColor : uint8
{
	Red		UMETA(DisplayName = "Red"),
	Yellow	UMETA(DisplayName = "Yellow"),
	Blue	UMETA(DisplayName = "Blue")
};


//main setup
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class METALGEARSEXAM_API UPlayerEquipComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerEquipComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	//Setting up all the variables here
	//State Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Equipment|State")
	EEquipState CurrentEquipState;

	UFUNCTION(BlueprintCallable, Category = "Equipment|State")
	void SetEquipState(EEquipState NewState);
	
	//Ammo Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment|Ammo")
	int32 CurrentAmmoKnife;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment|Ammo")
	int32 MaxAmmoKnife;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment|Ammo")
	int32 CurrentAmmoSilencer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment|Ammo")
	int32 MaxAmmoSilencer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment|Ammo")
	int32 CurrentAmmoDuckie;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment|Ammo")
	int32 MaxAmmoDuckie;
	
	//Keycard inventory Array
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment|Inventory")
	TArray<EKeycardColor> KeycardInventory;
	
	//Cds and timers
	//Duckie fire rate
	UPROPERTY(EditAnywhere, Category = "Equipment|Combat")
	float FireRate_Duckie;
	//handle for timer
	FTimerHandle TimerHandle_DuckieCooldown;
	void ResetDuckieCooldown();
	
	UPROPERTY(BlueprintReadOnly, Category = "Equipment|Combat")
	bool bCanShoot;
	
	UPROPERTY(EditAnywhere, Category = "Equipment|Combat")
	float FireRate_Gun;
	
	UPROPERTY(BlueprintReadOnly, Category = "Equipment|Combat")
	bool bCanThrowKnife;
	
	UPROPERTY(EditAnywhere, Category = "Equipment|Combat")
	float FireRate_Knife;
	
	UPROPERTY(BlueprintReadOnly, Category = "Equipment|Combat")
	bool bCanBomb;
	
	UPROPERTY(BlueprintReadOnly, Category = "Equipment|Combat")
	bool bIsThrowing;
	
	//Timerhandles (to replace Bp delays)
	FTimerHandle TimerHandle_GunCooldown;
	FTimerHandle TimerHandle_KnifeCooldown;
	
	//c++ input functions
	void TryShootGun();
	void TryThrowKnife();
	void InputDuckieStarted();
	void InputDuckieCompleted();
	
	// Timer Callbacks
	void ResetGunCooldown();
	void ResetKnifeCooldown();
	
	//add ammo function (making ammo more modular so not 3 seperate values i have to update)
	//weapontypes pistol = 0, kunai = 1, duckie = 2
	UFUNCTION(BlueprintCallable, Category = "Equipment|Ammo")
	void AddAmmo(int32 WeaponType, int32 Amount);
	
	//adds keycard hopefully
	UFUNCTION(BlueprintCallable, Category = "Equipment|Inventory")
	void AddKeycard(EKeycardColor CardColor);
	
	//bridges for visuals, equipchanges and visibility changes. uses uproperty isntead of function
	// Runs updateweapons
	UPROPERTY(BlueprintAssignable, Category = "Equipment|Events")
	FEquipEventSignature OnEquipStateChanged;

	// Runs update all ammo
	UPROPERTY(BlueprintAssignable, Category = "Equipment|Events")
	FEquipEventSignature OnUpdateAmmoUI;

	//delegate keycard when changed
	UPROPERTY(BlueprintAssignable, Category = "Equipment|Events")
	FEquipEventSignature OnKeycardInventoryChanged;

	// Function that will fire off the audio queues
	UPROPERTY(BlueprintAssignable, Category = "Equipment|Events")
	FShootEffectsSignature OnPlayShootEffects;

	// Function that ensures knife disapears while throwing
	UPROPERTY(BlueprintAssignable, Category = "Equipment|Events")
	FEquipEventSignature OnSpawnKnifeToss;

	// spawns duckie in hand and runs aim
	UPROPERTY(BlueprintAssignable, Category = "Equipment|Events")
	FEquipEventSignature OnStartDuckieAim;

	// releases the duckie and turns on physics again.
	UPROPERTY(BlueprintAssignable, Category = "Equipment|Events")
	FEquipEventSignature OnReleaseDuckie;
	
	UPROPERTY(BlueprintAssignable, Category = "Equipment|Events")
	FEquipEventSignature OnGunEmptyClick;
};
