// Fill out your copyright notice in the Description page of Project Settings.

#include "SplineProps.h"

// Sets default values
ASplineProps::ASplineProps() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootCompponent");
	SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
	if (SplineComponent && RootComponent)
	{
		SplineComponent->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void ASplineProps::BeginPlay() {
	AActor::BeginPlay();

}

// Called every frame
void ASplineProps::Tick(float DeltaTime) {
	AActor::Tick(DeltaTime);

}

void ASplineProps::OnConstruction(const FTransform& Transform) {
	AActor::OnConstruction(Transform);
	if (!DefaultMesh) { return; } // if there no mesh no need to go farther
	for (int i = 0; i < SplineComponent->GetNumberOfSplinePoints() - 1; i++)
	{
		// Get  vectors for making meshes on certain points
		FVector StartPoint = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
		FVector NextPoint = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
		FVector NextTangent = SplineComponent->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
		// Making meshes on spline points
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		if (SplineMeshComponent)
		{
			SplineMeshComponent->SetStaticMesh(DefaultMesh);
			SplineMeshComponent->SetMobility(EComponentMobility::Movable);
			SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
			SplineMeshComponent->RegisterComponentWithWorld(GetWorld());
			SplineMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
			//Locate mesh in the right position 
			SplineMeshComponent->SetStartAndEnd(StartPoint, StartTangent, NextPoint, NextTangent, true);
			SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			SplineMeshComponent->SetForwardAxis(SplineMeshAxis);
			// Get matirial from MeshMaterialInstanceMap else use default material
			if (SplineOverrideMap.Contains(i))
			{
				//Set Mesh
				if (SplineOverrideMap.Find(i)->Mesh) 
				{
				SplineMeshComponent->SetStaticMesh(SplineOverrideMap.Find(i)->Mesh);
				}
				// Set Material
				if (SplineOverrideMap.Find(i)->MaterialInstance)
				{
					SplineMeshComponent->SetMaterial(0, SplineOverrideMap.Find(i)->MaterialInstance);
				}
				// Set Axis
				SplineMeshComponent->SetForwardAxis(SplineOverrideMap.Find(i)->SplineMeshAxis);
			}
			else if (DefaultMaterialInstance)
			{
				SplineMeshComponent->SetMaterial(0, DefaultMaterialInstance);
			}
		}
	}
}

