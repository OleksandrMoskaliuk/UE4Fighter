// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components\StaticMeshComponent.h"
#include "Components\SkeletalMeshComponent.h"

#include "PhysicsConstraints.generated.h"

UCLASS()
class UE4FIGHTER_API APhysicsProp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APhysicsProp();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Physics")
		class UPhysicsConstraintComponent* PhysicsConstraint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Physics")
		class UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Physics")
		class UStaticMeshComponent* MovableMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Physics")
		class USkeletalMeshComponent* SkeletalMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
