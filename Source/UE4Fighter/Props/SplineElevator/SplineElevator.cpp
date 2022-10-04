// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineElevator.h"

// Sets default values
ASplineElevator::ASplineElevator() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));

	SplineComponent->SetupAttachment(GetRootComponent());
	MeshComponent->AttachTo(SplineComponent);
	TriggerBox->AttachTo(MeshComponent);
	AutoRestartSwitcher = false;
	AutoStart = false;
	RestartOption = ASplineElevatorRestartOption::OFF;
}

// Called when the game starts or when spawned
void ASplineElevator::BeginPlay() {
	AActor::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ASplineElevator::TriggerBeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ASplineElevator::TriggerEndOverlap);

	// setup for MovementTimeline
	FOnTimelineFloat ProgressFunction;
	FOnTimelineEvent FinishTimilineFunction;
	ProgressFunction.BindUFunction(this, TEXT("StartProcessMovementTimeline"));
	FinishTimilineFunction.BindUFunction(this, TEXT("EndProcessMovementTimeline"));
	if (MovementCurve)
	{
		//MovementTimeline.SetTimelinePostUpdateFunc();
		MovementTimeline.AddInterpFloat(MovementCurve, ProgressFunction);
		MovementTimeline.SetTimelineFinishedFunc(FinishTimilineFunction);
		MovementTimeline.SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
	}
	if (AutoStart) 
	{
		MovementTimeline.Play();
	}
}

// Called every frame
void ASplineElevator::Tick(float DeltaTime) {
	AActor::Tick(DeltaTime);
	if (MovementTimeline.IsPlaying())
	{
		MovementTimeline.TickTimeline(DeltaTime);
	}


}

void ASplineElevator::StartProcessMovementTimeline(float MovementCurveValue) {
	const float SplineLength = SplineComponent->GetSplineLength();
	// MovementCurveValue variation 0.001....1
	// Get location along all spline points
	FVector SplineComponentLocation = SplineComponent->GetLocationAtDistanceAlongSpline(MovementCurveValue * SplineLength, ESplineCoordinateSpace::World);
	FRotator SplineComponentRotation = SplineComponent->GetRotationAtDistanceAlongSpline(MovementCurveValue * SplineLength, ESplineCoordinateSpace::World);
	SplineComponentRotation.Pitch = 0;
	MeshComponent->SetWorldLocationAndRotation(SplineComponentLocation, SplineComponentRotation);

}

void ASplineElevator::EndProcessMovementTimeline() {
	GEngine->AddOnScreenDebugMessage(200, 0.1f, FColor::Yellow, __FUNCTION__);
	// AutoRestart
	if (AutoRestart)
	{
		if (AutoRestartSwitcher)
		{
			MovementTimeline.PlayFromStart();
			AutoRestartSwitcher = false;
		}
		else
		{
			MovementTimeline.Reverse();
			AutoRestartSwitcher = true;
		}
	}
	// AutoRestartFromEnd
 if (AutoRestartFromEnd)
	{
		if (!MovementTimeline.IsPlaying())
		{
			MovementTimeline.PlayFromStart();
		}
	}
	//AutoRestartFromBegin
	if (AutoRestartFromBegin)
	{
		if (!MovementTimeline.IsPlaying())
		{
			MovementTimeline.PlayFromStart();
		}
	}
}

void ASplineElevator::TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	GEngine->AddOnScreenDebugMessage(400, 2.0f, FColor::Green, __FUNCTION__);
	if(!AutoStart)
	{
		MovementTimeline.Play();
	}
	
}

void ASplineElevator::TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	GEngine->AddOnScreenDebugMessage(400, 2.0f, FColor::Green, __FUNCTION__);
	if (!MovementTimeline.IsReversing() && !AutoRestart && !ReverseAutoRestart)
	{
		MovementTimeline.Reverse();
	}

}

