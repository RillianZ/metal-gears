#include "VentDoor.h"
#include "Kismet/GameplayStatics.h"

AVentDoor::AVentDoor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	VentFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VentFrameMesh"));
	VentFrameMesh->SetupAttachment(Root);

	VentDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VentDoorMesh"));
	VentDoorMesh->SetupAttachment(VentFrameMesh);
}

void AVentDoor::BeginPlay()
{
	Super::BeginPlay();
}

void AVentDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LerpAlpha < 1.0f)
	{
		LerpAlpha += DeltaTime;
		float NewRotation = FMath::Lerp(CurrentRotation, TargetRotation, LerpAlpha);
		VentDoorMesh->SetRelativeRotation(FRotator(0.0f, NewRotation, 0.0f));
	}
}

void AVentDoor::Interact_Implementation(AActor* PlayerRef)
{
	if (bIsOpen)
		CloseDoor();
	else
		OpenDoor();
}

void AVentDoor::OpenDoor()
{
	CurrentRotation = VentDoorMesh->GetRelativeRotation().Yaw;
	TargetRotation = CurrentRotation + RotationAmount;
	LerpAlpha = 0.0f;

	bIsOpen = true;

	if (OpenSound)
		UGameplayStatics::PlaySoundAtLocation(this, OpenSound, GetActorLocation());
}

void AVentDoor::CloseDoor()
{
	CurrentRotation = VentDoorMesh->GetRelativeRotation().Yaw;
	TargetRotation = 0.0f;
	LerpAlpha = 0.0f;

	bIsOpen = false;

	if (CloseSound)
		UGameplayStatics::PlaySoundAtLocation(this, CloseSound, GetActorLocation());
}