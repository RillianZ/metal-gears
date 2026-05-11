#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StealthDetectionComponent.generated.h"

//Doing a declare aka event dispatcher, to shout out when 100% detection
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDetectionMaxedSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class METALGEARSEXAM_API UStealthDetectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this components properties
	UStealthDetectionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Detection variables
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stealth")
	float CurrentDetection;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stealth")
	float BaseDetectionRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stealth")
	float CrouchStealthModifier;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stealth")
	int32 ObserverCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stealth")
	bool bIsHidden;
	
	//Detection Functions
	UFUNCTION(BlueprintCallable, Category = "Stealth")
	void AddObserver();
	
	UFUNCTION(BlueprintCallable, Category = "Stealth")
	void RemoveObserver();
	
	UPROPERTY(BlueprintAssignable, Category = "Stealth")
	FOnDetectionMaxedSignature OnDetectionMaxed;
};
