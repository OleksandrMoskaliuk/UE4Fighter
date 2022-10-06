// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MovingSplineActor.h"
#include "Particles/ParticleSystemComponent.h"

#include "ParticleMovingSplineActor.generated.h"

UCLASS()
class UE4FIGHTER_API AParticleMovingSplineActor : public AMovingSplineActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline", meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* ParticleComponent;

public:
	// Sets default values for this character's properties
	AParticleMovingSplineActor();

public:
	// Called every frame
	void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	// Trigger by box overlap or AutoStartFromBeginPlay
	void StartProcessMovementTimeline(float MovementCurveValue) override;
	
	//Trigger by box overlap or AutoStartFromBeginPlay see AMovingSplineActor
	void TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Trigger by box overlap or AutoStartFromBeginPlay see AMovingSplineActor
	void TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
