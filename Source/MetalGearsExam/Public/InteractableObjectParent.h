#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interact.h"
#include "InteractableObjectParent.generated.h"

UCLASS()
class METALGEARSEXAM_API AInteractableObjectParent 
	: public AActor, public IInteract
{
	GENERATED_BODY()

public:
	AInteractableObjectParent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	virtual void Interact_Implementation(AActor* PlayerRef) override;
};