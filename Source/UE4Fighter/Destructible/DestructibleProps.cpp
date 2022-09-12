// Fill out your copyright notice in the Description page of Project Settings.


#include "../Destructible/DestructibleProps.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "../Character/UE4FighterCharacter.h"

// Sets default values
ADestructibleProps::ADestructibleProps()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));

	DestructibleComponent = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Destructible Component"));
	DestructibleComponent->SetupAttachment(this->RootComponent);
	DestructibleComponent->SetNotifyRigidBodyCollision(true);

	TriggerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Component"));
	TriggerComponent->SetupAttachment(RootComponent);

	IsDestroyed = false;
	IsTriggerEnable = false;
	MaxHealth = 1.f;
	DefaultDamage = 1.f;
	DefaulImpulse = 1.f;

}


void ADestructibleProps::Destroy() {
}

// Called when the game starts or when spawned
void ADestructibleProps::BeginPlay()
{
	AActor::BeginPlay();
	DestructibleComponent->OnComponentHit.AddDynamic(this, &ADestructibleProps::Damage);
	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ADestructibleProps::Trigger);
	CurrentHealth = MaxHealth;
	
}

// Called every frame
void ADestructibleProps::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

}

void ADestructibleProps::Damage(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	//GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::White, __FUNCTION__);

	

}

void ADestructibleProps::Trigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {


	if (OtherActor)
	{
		//UBoxComponent* CollisionBox = Cast<UBoxComponent>(OtherActor);
		GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Blue, OtherActor->GetName());
		AUE4FighterCharacter* Player = Cast<AUE4FighterCharacter>(OtherActor);
		if (Player != NULL)
		{
			 if(Player->GetIsAnimationBlended()){
					DestructibleComponent->ApplyDamage(10000.f, DestructibleComponent->GetComponentLocation(), OtherComp->GetPhysicsLinearVelocity(), 10000.f);
				}
		}	
	}
	
}

