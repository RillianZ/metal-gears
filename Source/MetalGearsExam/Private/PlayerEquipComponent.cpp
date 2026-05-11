#include "PlayerEquipComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UPlayerEquipComponent::UPlayerEquipComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	// Default State on lvl start should be unarmed, and ammo to 0
	CurrentEquipState = EEquipState::Unarmed;

	// Default Ammo
	CurrentAmmoKnife = 0;
	MaxAmmoKnife = 10;
	
	CurrentAmmoSilencer = 0;
	MaxAmmoSilencer = 10;

	CurrentAmmoDuckie = 0;
	MaxAmmoDuckie = 10;
	
	// Default Cooldowns & States (should knife be slower than gun?)
	bCanShoot = true;
	FireRate_Gun = 0.5f;

	bCanThrowKnife = true;
	FireRate_Knife = 0.5f;

	bCanBomb = true;
	bIsThrowing = false;
	FireRate_Duckie = 1.5f;
	
}


// Called when the game starts
void UPlayerEquipComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

//All major functions below, replacing every action in the BP for inventory
void UPlayerEquipComponent::SetEquipState(EEquipState NewState)
{
	CurrentEquipState = NewState;
	OnEquipStateChanged.Broadcast(); 
}

//we use a switch to check which one we add to then we run updateammoui bp.
void UPlayerEquipComponent::AddAmmo(int32 WeaponType, int32 Amount)
{
	switch (WeaponType)
	{
	case 0: //silencerpistol
	CurrentAmmoSilencer = FMath::Clamp(CurrentAmmoSilencer + Amount, 0, MaxAmmoSilencer);
		break;
		
	case 1: //knife
		CurrentAmmoKnife =FMath::Clamp(CurrentAmmoKnife + Amount, 0, MaxAmmoKnife);
		break;
		
	case 2: //Duckkiiieee
		CurrentAmmoDuckie = FMath::Clamp((CurrentAmmoDuckie + Amount), 0, MaxAmmoDuckie);
		break;
	}
	OnUpdateAmmoUI.Broadcast();
}

//Setting up each individual to match the branch checks in bp
//checks equipstate, checks ammo, checks bool
void UPlayerEquipComponent::TryShootGun()
{
	if (CurrentEquipState == EEquipState::Pistol && bCanShoot)
	{
		bCanShoot = false;
		//Same timer as below
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_GunCooldown, this,
			&UPlayerEquipComponent::ResetGunCooldown, FireRate_Gun, false);
		
		//if ammo more than 0, run linetrace and broadcast
		if (CurrentAmmoSilencer > 0)
		{
		CurrentAmmoSilencer--;
		OnUpdateAmmoUI.Broadcast();
					
		//Linetrace used instead of projectile for gun
		//Used Gemini here, had no idea how to setup the linetrace in c++
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
			{
			//makes some variables
			FVector CameraLoc;
			FRotator CameraRot;
			PC->GetPlayerViewPoint(CameraLoc, CameraRot);
			//Shoots a beam, ziiiiing, gives hitresult.
			FVector EndTrace = CameraLoc + (CameraRot.Vector() * 10000.0f); 
			FHitResult HitResult;
			//ignores self
			FCollisionQueryParams TraceParams;
			TraceParams.AddIgnoredActor(GetOwner()); 
			//actual info channel for collison runs on visibility channel, starts at cameraloc.
			bool bHit = GetWorld()->LineTraceSingleByChannel
			(HitResult, CameraLoc, EndTrace, ECC_Visibility, TraceParams);
			//if it hits actors, ouchies
			if (bHit && HitResult.GetActor())
				{
					// we apply native damage here instead of bp method of checking for tags.
					UGameplayStatics::ApplyDamage(HitResult.GetActor(), 100.0f, PC, GetOwner(), nullptr);
				}
			// links to the bp and original effects.
			OnPlayShootEffects.Broadcast(bHit, HitResult.ImpactPoint, HitResult.GetActor());
			}
		}
		//if no ammo
		else
		{
			OnGunEmptyClick.Broadcast();
		}
	}
		
}

//same system as below
void UPlayerEquipComponent::ResetGunCooldown()
{
	bCanShoot=true;
}

//If we in knife state and we can throw knife and we have ammo, 
//set fcanthrowknife, remove 1 ammo, update ui and spawn a knife after cd is done.
//this TimerManager code part was provided using gemini. i did not know how delay worked in c++
void UPlayerEquipComponent::TryThrowKnife()
{
	if (CurrentEquipState == EEquipState::Knife && bCanThrowKnife && CurrentAmmoKnife > 0)
	{
		//sets the variabs
		bCanThrowKnife = false;
		CurrentAmmoKnife--;
		OnUpdateAmmoUI.Broadcast();
		// This timer here
		//it takes the resetknifecooldown and ftime i put in header, 
		//sets a timer using that and the firerateknife variable, when the timer runs out it pings the resetknifecd
		//which sets canthrowknife to true again, functionally the same as the delay.
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_KnifeCooldown, this, 
			&UPlayerEquipComponent::ResetKnifeCooldown, FireRate_Knife, false);

		//Spawns the knife
		OnSpawnKnifeToss.Broadcast(); 
		
	}
}

//Sets canthrowknife to true when resetknifecd has its timer run the firerate knife time.
void UPlayerEquipComponent::ResetKnifeCooldown()
{
	bCanThrowKnife=true;
}

//if we can bomb and we have ammo, set throwing true and start aiming
void UPlayerEquipComponent::InputDuckieStarted()
{
	if (bCanBomb && CurrentAmmoDuckie > 0)
	{
		bCanBomb = false;
		bIsThrowing = true;
		OnStartDuckieAim.Broadcast();
	}
}

//when aim is released, if still in isthrowingstate remove ammo, update ui, release duckie, and set throwing to false.
void UPlayerEquipComponent::InputDuckieCompleted()
	{
		if (bIsThrowing)
		{
			bIsThrowing = false;
			CurrentAmmoDuckie--;
			OnUpdateAmmoUI.Broadcast();
			OnReleaseDuckie.Broadcast();
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_DuckieCooldown, 
				this, &UPlayerEquipComponent::ResetDuckieCooldown, FireRate_Duckie, false);
		}
	}
//sets canbomb to true when cd is met.
void UPlayerEquipComponent::ResetDuckieCooldown()
{
	bCanBomb = true;
}

void UPlayerEquipComponent::AddKeycard(EKeycardColor CardColor)
{
	//adds if we dont have one from before
	if (!KeycardInventory.Contains(CardColor))
	{
		KeycardInventory.Add(CardColor);
		//updates UI i hope
		OnKeycardInventoryChanged.Broadcast(); 
	}
}

