#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interact.generated.h"

UINTERFACE(MinimalAPI)
class UInteract : public UInterface
{
	GENERATED_BODY()
};

class METALGEARSEXAM_API IInteract
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	void Interact(AActor* PlayerRef);
};