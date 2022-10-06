// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MovingSplineActor.h"

#include "MovingMeshAlongSplineActor.generated.h"


UCLASS()
class UE4FIGHTER_API AMovingMeshAlongSplineActor : public AMovingSplineActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MeshComponent;

public:
	// Sets default values for this character's properties
	AMovingMeshAlongSplineActor();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Trigger by box overlap or AutoStartFromBeginPlay
	virtual void StartProcessMovementTimeline(float MovementCurveValue) override;

};
