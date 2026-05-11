// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "MainPlayerChar.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// Components
	IconDistance = CreateDefaultSubobject<USphereComponent>(TEXT("IconDistance"));
	IconDistance->SetupAttachment(GetRootComponent());
	
	KnifeIcon = CreateDefaultSubobject<UWidgetComponent>(TEXT("KinfeIcon"));
	KnifeIcon->SetupAttachment(RootComponent);
	
	// Default State
	KnifeIcon->SetVisibility(false);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IconDistance)
	{
		UE_LOG(LogTemp, Error, TEXT("IconDistance is null on %s"), *GetName());
		return;
	}
	
	IconDistance->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);
	IconDistance->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnOverlapEnd);
	
	if (KnifeIcon)
	{
		KnifeIcon->SetVisibility(false);
	}
}

void AEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AMainPlayerChar>(OtherActor))
	{
		KnifeIcon->SetVisibility(true);
	}
}

void AEnemy::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (Cast<AMainPlayerChar>(OtherActor))
	{
		KnifeIcon->SetVisibility(false);
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

