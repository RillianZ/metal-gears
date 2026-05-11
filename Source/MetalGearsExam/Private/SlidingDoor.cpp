#include "SlidingDoor.h"
#include "Components/StaticMeshComponent.h"

ASlidingDoor::ASlidingDoor()
{
    PrimaryActorTick.bCanEverTick = true;

    LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoor"));
    LeftDoor->SetupAttachment(RootComponent);

    RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoor"));
    RightDoor->SetupAttachment(RootComponent);

    DoorLightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorLightMesh"));
    DoorLightMesh->SetupAttachment(RootComponent);

    bIsOpening = false;
    CurveAlpha = 0.f;
    SlideDistance = 100.f;
    DoorSpeed = 1.f;
    MaterialSlotIndex = 0;
}

void ASlidingDoor::BeginPlay()
{
    Super::BeginPlay();

    LeftClosedPos = LeftDoor->GetRelativeLocation();
    RightClosedPos = RightDoor->GetRelativeLocation();

    LeftOpenPos = LeftClosedPos + FVector(0.f, -SlideDistance, 0.f);
    RightOpenPos = RightClosedPos + FVector(0.f, SlideDistance, 0.f);
}

void ASlidingDoor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    float Direction = bIsOpening ? 1.f : -1.f;
    CurveAlpha = FMath::Clamp(CurveAlpha + DeltaTime * DoorSpeed * Direction, 0.f, 1.f);

    float SmoothAlpha = CurveAlpha * CurveAlpha * (3.f - 2.f * CurveAlpha);

    FVector NewLeft = FMath::Lerp(LeftClosedPos, LeftOpenPos, SmoothAlpha);
    FVector NewRight = FMath::Lerp(RightClosedPos, RightOpenPos, SmoothAlpha);

    LeftDoor->SetRelativeLocation(NewLeft);
    RightDoor->SetRelativeLocation(NewRight);
}

void ASlidingDoor::ToggleDoor()
{
    bIsOpening = !bIsOpening;

    if (DoorLightMesh)
        DoorLightMesh->SetMaterial(MaterialSlotIndex, bIsOpening ? OpenMaterial : ClosedMaterial);
}