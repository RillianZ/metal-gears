#include "InteractableObjectParent.h"

AInteractableObjectParent::AInteractableObjectParent()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

void AInteractableObjectParent::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractableObjectParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableObjectParent::Interact_Implementation(AActor* PlayerRef)
{
	UE_LOG(LogTemp, Warning, TEXT("Interact called from C++ parent by %s"),
		*PlayerRef->GetName());
}