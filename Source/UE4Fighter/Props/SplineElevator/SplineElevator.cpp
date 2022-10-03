// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/SplineElevator/SplineElevator.h"

// Sets default values
ASplineElevator::ASplineElevator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASplineElevator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASplineElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

