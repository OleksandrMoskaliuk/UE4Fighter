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
	DefaulImpulse = 10000.f;

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

void ADestructibleProps::TimerTrigger() {
	
	if (IsDestroyed)
		return;
	IsDestroyed = true;
	GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Red, "Kabooooom!");
	DestructibleComponent->ApplyDamage(10000.f, HitLocation, ImpulseDirection, DefaulImpulse);
}

void ADestructibleProps::CountDownTimer() {

	if (IsDestroyed)
	{
		return;
	}
	TimeToDestroy -= 0.2f;
	if (TimeToDestroy <= 0.f) 
	{
		IsDestroyed = true;
		GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Red, "Kabooooom!");
		DestructibleComponent->ApplyDamage(10000.f, HitLocation, ImpulseDirection, DefaulImpulse);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Red,  FString::SanitizeFloat(TimeToDestroy) + "s, to cube explosion");
	}
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
	if (IsDestroyed){
		return;
	}
	if (OtherActor)
	{
		//UBoxComponent* CollisionBox = Cast<UBoxComponent>(OtherActor);
		AUE4FighterCharacter* Player = Cast<AUE4FighterCharacter>(OtherActor);
		if (Player != NULL)
		{
			 // fill params for ADestructibleProps::TimerTrigger() 
				HitLocation = DestructibleComponent->GetComponentLocation();
				ImpulseDirection = OtherComp->GetPhysicsLinearVelocity();
				bool IsTimerActive = GetWorld()->GetTimerManager().IsTimerActive(this->DestroyTimer);
				if (!IsDestroyed && !IsTimerActive)
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Red, "TimerHasBeenStarted");
					this->TimeToDestroy = FMath::RandRange(2.f, 9.f);
					GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &ADestructibleProps::CountDownTimer, 0.2f,true);
				}
		}
	}
}

