#pragma once

#include "CoreMinimal.h"
#include "InteractableObjectParent.h"
#include "SlidingDoor.generated.h"

class UStaticMeshComponent;

UCLASS()
class METALGEARSEXAM_API ASlidingDoor : public AInteractableObjectParent
{
	GENERATED_BODY()

public:
	ASlidingDoor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LeftDoor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* RightDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Door")
	UStaticMeshComponent* DoorLightMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Door")
	UMaterialInterface* ClosedMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Door")
	UMaterialInterface* OpenMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Door")
	int32 MaterialSlotIndex;

	bool bIsOpening;
	float CurveAlpha;
	float SlideDistance;
	float DoorSpeed;

	FVector LeftClosedPos;
	FVector LeftOpenPos;
	FVector RightClosedPos;
	FVector RightOpenPos;

public:
	UFUNCTION(BlueprintCallable)
	void ToggleDoor();
};