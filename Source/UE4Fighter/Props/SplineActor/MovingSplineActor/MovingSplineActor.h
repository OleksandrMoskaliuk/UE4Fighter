// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"

#include "MovingSplineActor.generated.h"

UENUM(BlueprintType)
enum class ASplineElevatorRestartOption : uint8 {
	OFF			UMETA(DisplayName = "Only player interraction can restart elevator"),
	DEFAULT			UMETA(DisplayName = "Default auto restart elevator"),
	FROM_BEGIN			UMETA(DisplayName = "Restart elevator from start spline point"),
	FROM_END			UMETA(DisplayName = "Restart elevator from start spline from end spline point")
};

UCLASS()
class UE4FIGHTER_API AMovingSplineActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spline", meta = (AllowPrivateAccess = "true"))
		UCurveFloat* MovementCurve;
	
	/** Turn on spline elevator from begin playing  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Restart Option", meta = (AllowPrivateAccess = "true"))
		bool AutoStartFromBeginPlay;

	// Plays movement 
	FTimeline MovementTimeline;

	// holds last spline location from StartProcessMovementTimeline
	FVector SplineComponentLocation;
 
	// holds last spline rotation from StartProcessMovementTimeline
	FRotator SplineComponentRotation;

	// Using for auto restart spline elevator logick  
	bool AutoRestartSwitcher;

public:	
	// Sets default values for this actor's properties
	AMovingSplineActor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetSplineLocation();

	FRotator GetSplineRotation();

	UFUNCTION()
		virtual void StartProcessMovementTimeline(float MovementCurveValue);

	UFUNCTION()
		void EndProcessMovementTimeline();

	//Trigger by box overlap or AutoStartFromBeginPlay
	UFUNCTION()
		virtual void TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Trigger by box overlap or AutoStartFromBeginPlay
	UFUNCTION()
		virtual void TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline", meta = (AllowPrivateAccess = "true"))
		USplineComponent* SplineComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* TriggerBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Restart Option", meta = (AllowPrivateAccess = "true"))
		ASplineElevatorRestartOption RestartOption;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
