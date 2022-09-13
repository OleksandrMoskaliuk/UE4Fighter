// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "DestructibleComponent.h"

#include "DestructibleProps.generated.h"

UCLASS()
class UE4FIGHTER_API ADestructibleProps : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADestructibleProps();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Destructible")
		class UBoxComponent* TriggerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Destructible")
		class UDestructibleComponent* DestructibleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Destructible")
		bool IsDestroyed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Destructible")
		bool IsTriggerEnable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Destructible")
		float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Destructible")
		float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Destructible")
		float DefaultDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Destructible")
		float DefaulImpulse;

private:
	FTimerHandle DestroyTimer;
	// variables for  ADestructibleProps::TimerTrigger
	FVector HitLocation;
	FVector ImpulseDirection;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Will destroy actor when timer trigger
	void TimerTrigger();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void Damage(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	 void Trigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void Destroy();

};
