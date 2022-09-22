// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExampleInterface.h"
#include "SwitchByTagProp.generated.h"

UCLASS()
class UE4FIGHTER_API ASwitchByTagProp : public AActor, public IExampleInterface
{
	GENERATED_BODY()
	
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BaseMesh;

public:	
	// Sets default values for this actor's properties
	ASwitchByTagProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact(); //prototype declaration
	virtual void Interact_Implementation() override; //actual implemantation

};
