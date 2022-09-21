// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorInterfaceTest.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AActorInterfaceTest::AActorInterfaceTest() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent); 
	bIsBig = false;
}

// Called when the game starts or when spawned
void AActorInterfaceTest::BeginPlay() {
	Super::BeginPlay();

}

// Called every frame
void AActorInterfaceTest::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	
}

void AActorInterfaceTest::Jump() {
	BaseMesh->AddImpulse(FVector(0.f, 0.f, 200000.f), FName(TEXT("Impulse")));
}

void AActorInterfaceTest::Interact_Implementation() {
	if (bIsBig) 
	{
		//this->BaseMesh->SetWorldScale3D(FVector(2.f, 2.f, 2.f));
		BaseMesh->AddImpulse(FVector(0.f, 0.f, 200000.f), FName(TEXT("Impulse")));
		GetWorld()->GetTimerManager().SetTimer(JumpTimer, this, &AActorInterfaceTest::Jump, 3.f, true);
	}
	else 
	{
		//this->BaseMesh->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
	}
	bIsBig = !bIsBig;

}

