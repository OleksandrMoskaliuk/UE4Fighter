// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExampleInterface.h"

#include "ActorInterfaceTest.generated.h"

UCLASS()
class UE4FIGHTER_API AActorInterfaceTest : public AActor, public IExampleInterface
{
	GENERATED_BODY()
	
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BaseMesh;

	 bool bIsBig;
		bool bIsScaled;
		FTimerHandle JumpTimer;

public:	

	// Sets default values for this actor's properties
	AActorInterfaceTest();

// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// add impulse to mesh by timer
	void Jump();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact(); //prototype declaration
	 virtual void Interact_Implementation() override; //actual implemantation

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
