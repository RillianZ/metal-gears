#include "Door.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LerpAlpha < 1.0f)
	{
		LerpAlpha += DeltaTime;
		float NewRotation = FMath::Lerp(CurrentRotation, TargetRotation, LerpAlpha);
		Mesh->SetRelativeRotation(FRotator(0.0f, NewRotation, 0.0f));
	}
}

void ADoor::Interact_Implementation(AActor* PlayerRef)
{
	if (bIsOpen)
		CloseDoor();
	else
		OpenDoor(PlayerRef);
}

void ADoor::OpenDoor(AActor* PlayerRef)
{
	FVector DoorForward = GetActorForwardVector();
	FVector ToPlayer = (PlayerRef->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	float Dot = FVector::DotProduct(DoorForward, ToPlayer);

	float Direction = (Dot > 0) ? 1.0f : -1.0f;

	CurrentRotation = Mesh->GetRelativeRotation().Yaw;
	TargetRotation = CurrentRotation + (RotationAmount * Direction);
	LerpAlpha = 0.0f;

	bIsOpen = true;

	if (OpenSound)
		UGameplayStatics::PlaySoundAtLocation(this, OpenSound, GetActorLocation());
}

void ADoor::CloseDoor()
{
	CurrentRotation = Mesh->GetRelativeRotation().Yaw;
	TargetRotation = 0.0f;
	LerpAlpha = 0.0f;

	bIsOpen = false;

	if (CloseSound)
		UGameplayStatics::PlaySoundAtLocation(this, CloseSound, GetActorLocation());
}