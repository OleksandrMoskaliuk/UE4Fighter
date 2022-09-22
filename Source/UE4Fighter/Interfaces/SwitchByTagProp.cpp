// Fill out your copyright notice in the Description page of Project Settings.


#include "SwitchByTagProp.h"
#include "EngineUtils.h"
#include "ExampleInterface.h"
#include "Engine/Engine.h"


// Sets default values
ASwitchByTagProp::ASwitchByTagProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetCollisionProfileName(FName(TEXT("Enemy")));
}

// Called when the game starts or when spawned
void ASwitchByTagProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASwitchByTagProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASwitchByTagProp::Interact_Implementation() {
	for (TActorIterator<AActor> ActorIterator(GetWorld()); ActorIterator; ++ActorIterator) 
	{
		AActor* Actor = *ActorIterator;
		if (Actor && Actor != this && Actor->GetClass()->ImplementsInterface(UExampleInterface::StaticClass())) 
		{
			/*for (FName Tag : Actor->Tags) 
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, Tag.ToString());
			}*/

			// only AddOnScreenDebugMessage when actor has a specific tag
			for (FName Tag : ToggleTags) 
			{
				if (Actor->ActorHasTag(Tag)) 
				{
					GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, Tag.ToString());
					IExampleInterface::Execute_Interact(Actor);
				}
			}
			
		}
	}

}

