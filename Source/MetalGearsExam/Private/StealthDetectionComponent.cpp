#include "StealthDetectionComponent.h"
#include "MainPlayerChar.h"

//for clamping
#include "Math/UnrealMathUtility.h"

//To refrence character
#include "GameFramework/Character.h"


// Sets default values for this component's properties
UStealthDetectionComponent::UStealthDetectionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//Default values taken from BP
	CurrentDetection = 0.0f;
	BaseDetectionRate = 70.0f;    
	CrouchStealthModifier = 0.5f;  
	ObserverCount = 0;
	bIsHidden = false;
}


// Called when the game starts
void UStealthDetectionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStealthDetectionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	bool bIsCrouched = false;
	if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
	{
		bIsCrouched = OwnerChar->bIsCrouched; 
	}
	// If watched
	if (ObserverCount > 0 && !bIsHidden)
	{
		//Makes a new main detection rate based on crouching status
		float ActiveRate = bIsCrouched ? (BaseDetectionRate * CrouchStealthModifier) : BaseDetectionRate;
		//adds detection meter
		CurrentDetection += (ActiveRate * DeltaTime);
	}
	else
	{
		//lowers detection 
		CurrentDetection -= (15.0f * DeltaTime);
	}

	//clamp between 1-100
	CurrentDetection = FMath::Clamp(CurrentDetection, 0.0f, 100.0f);

	//broadcast on 100% detection
	if (CurrentDetection >= 100.0f)
	{
		OnDetectionMaxed.Broadcast();
	}
}

//Same functions as in BP, adds or removes observers and locks it in min 0.
void UStealthDetectionComponent::AddObserver()
{
	ObserverCount++;
}

void UStealthDetectionComponent::RemoveObserver()
{
	ObserverCount--;
	if (ObserverCount < 0)
	{
		ObserverCount = 0;
	}
}

