#pragma once

#include "CoreMinimal.h"
#include "InteractableObjectParent.h"
#include "VentDoor.generated.h"

UCLASS()
class METALGEARSEXAM_API AVentDoor : public AInteractableObjectParent
{
	GENERATED_BODY()

public:
	AVentDoor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// How far the vent rotates when opening
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vent Door")
	float RotationAmount = 90.0f;

	// Sound to play when opening/closing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vent Door")
	USoundBase* OpenSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vent Door")
	USoundBase* CloseSound;

	// Internal state
	bool bIsOpen = false;
	float CurrentRotation = 0.0f;
	float TargetRotation = 0.0f;
	float LerpAlpha = 0.0f;

	// Called when the player interacts
	virtual void Interact_Implementation(AActor* PlayerRef) override;

private:
	void OpenDoor();
	void CloseDoor();

public:
	// Vent frame (static)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* VentFrameMesh;

	// Vent door (rotating part)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* VentDoorMesh;
};