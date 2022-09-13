// Fill out your copyright notice in the Description page of Project Settings.

#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "PhysicsConstraints.h"

// Sets default values
APhysicsProp::APhysicsProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	BaseMesh->SetupAttachment(RootComponent);

	MovableMesh = CreateDefaultSubobject<UStaticMeshComponent>("Movable Mesh");
	MovableMesh->SetupAttachment(RootComponent);
	MovableMesh->SetSimulatePhysics(true);
	MovableMesh->SetCollisionProfileName("Enemy");

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Skeletal Mesh");
	SkeletalMesh->SetupAttachment(RootComponent);
	SkeletalMesh->SetSimulatePhysics(true);
	SkeletalMesh->SetCollisionProfileName("Enemy");

	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Physics Constraints"));
	PhysicsConstraint->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APhysicsProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APhysicsProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

