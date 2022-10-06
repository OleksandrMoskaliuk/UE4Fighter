// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingMeshAlongSplineActor.h"

// Sets default values
AMovingMeshAlongSplineActor::AMovingMeshAlongSplineActor() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));

	MeshComponent->SetupAttachment(AMovingSplineActor::SplineComponent);
	TriggerBox->AttachTo(MeshComponent);

}

// Called when the game starts or when spawned
void AMovingMeshAlongSplineActor::BeginPlay() {
	AMovingSplineActor::BeginPlay();

}

void AMovingMeshAlongSplineActor::StartProcessMovementTimeline(float MovementCurveValue) {
	AMovingSplineActor::StartProcessMovementTimeline(MovementCurveValue);
	FRotator SplineRotation = GetSplineRotation();
	FVector SplineLocation = GetSplineLocation();
	SplineRotation.Pitch = 0;
	MeshComponent->SetWorldLocationAndRotation(SplineLocation, SplineRotation);

}

// Called every frame
void AMovingMeshAlongSplineActor::Tick(float DeltaTime) {
	AMovingSplineActor::Tick(DeltaTime);

}


