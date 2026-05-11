// Fill out your copyright notice in the Description page of Project Settings.


#include "HeavySlidingDoor.h"

// Sets default values
AHeavySlidingDoor::AHeavySlidingDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHeavySlidingDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHeavySlidingDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

