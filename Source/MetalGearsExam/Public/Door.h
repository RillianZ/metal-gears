#pragma once

#include "CoreMinimal.h"
#include "InteractableObjectParent.h"
#include "Door.generated.h"

UCLASS()
class METALGEARSEXAM_API ADoor : public AInteractableObjectParent
{
	GENERATED_BODY()

public:
	ADoor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// How much the door rotates when opening
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Door")
	float RotationAmount = 90.0f;

	// Sound to play when opening/closing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Door")
	USoundBase* OpenSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Door")
	USoundBase* CloseSound;

	// Internal state
	bool bIsOpen = false;
	float CurrentRotation = 0.0f;
	float TargetRotation = 0.0f;
	float LerpAlpha = 0.0f;

	// Called when the player interacts
	virtual void Interact_Implementation(AActor* PlayerRef) override;

private:
	void OpenDoor(AActor* PlayerRef);
	void CloseDoor();
};