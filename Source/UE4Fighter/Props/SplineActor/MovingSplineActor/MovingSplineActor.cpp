// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingSplineActor.h"

// Sets default values
AMovingSplineActor::AMovingSplineActor() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));

	SplineComponent->SetupAttachment(GetRootComponent());
	TriggerBox->AttachTo(SplineComponent);

	AutoRestartSwitcher = false;
	AutoStartFromBeginPlay = false;
	RestartOption = ASplineElevatorRestartOption::OFF;
}

// Called when the game starts or when spawned
void AMovingSplineActor::BeginPlay() {
	AActor::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMovingSplineActor::TriggerBeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AMovingSplineActor::TriggerEndOverlap);

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
	if (AutoStartFromBeginPlay)
	{
		MovementTimeline.Play();
	}
}

// Called every frame
void AMovingSplineActor::Tick(float DeltaTime) {
	AActor::Tick(DeltaTime);
	if (MovementTimeline.IsPlaying())
	{
		MovementTimeline.TickTimeline(DeltaTime);
	}

}

FVector AMovingSplineActor::GetSplineLocation() {
	return SplineComponentLocation;
}

FRotator AMovingSplineActor::GetSplineRotation() {
	return SplineComponentRotation;
}

void AMovingSplineActor::StartProcessMovementTimeline(float MovementCurveValue) {
	const float SplineLength = SplineComponent->GetSplineLength();
	// MovementCurveValue variation 0.001....1
	// Get location along all spline points
	SplineComponentLocation = SplineComponent->GetLocationAtDistanceAlongSpline(MovementCurveValue * SplineLength, ESplineCoordinateSpace::World);
	SplineComponentRotation = SplineComponent->GetRotationAtDistanceAlongSpline(MovementCurveValue * SplineLength, ESplineCoordinateSpace::World);
	GEngine->AddOnScreenDebugMessage(1998, 1.0f, FColor::Magenta, "MovementCurveValue = " + FString::SanitizeFloat(MovementCurveValue));
	
}

void AMovingSplineActor::EndProcessMovementTimeline() {
	GEngine->AddOnScreenDebugMessage(200, 0.1f, FColor::Yellow, __FUNCTION__);
	switch (RestartOption)
	{
		case ASplineElevatorRestartOption::OFF:
			// in case if AutoStartFromBeginPlay execute once
			if (AutoStartFromBeginPlay && !AutoRestartSwitcher)
			{
				MovementTimeline.Reverse();
				AutoRestartSwitcher = true;
			} break;
		case ASplineElevatorRestartOption::DEFAULT:
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
		} break;
		case ASplineElevatorRestartOption::FROM_BEGIN:
		{
			if (!MovementTimeline.IsPlaying())
			{
				MovementTimeline.PlayFromStart();
			}
		}break;
		case ASplineElevatorRestartOption::FROM_END:
		{
			if (!MovementTimeline.IsReversing())
			{
				MovementTimeline.ReverseFromEnd();
			}
		} break;
		default:
			break;
	}


}

void AMovingSplineActor::TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	GEngine->AddOnScreenDebugMessage(400, 2.0f, FColor::Green, __FUNCTION__);
	switch (RestartOption)
	{
		case ASplineElevatorRestartOption::OFF:
		{
				MovementTimeline.Play();
		} break;
		case ASplineElevatorRestartOption::DEFAULT:
			if (!MovementTimeline.IsPlaying())
			{
				MovementTimeline.Play();
			}
		break;
		case ASplineElevatorRestartOption::FROM_BEGIN:
			if (!MovementTimeline.IsPlaying())
			{
				MovementTimeline.Play();
			}
			break;
		case ASplineElevatorRestartOption::FROM_END:
			if (!MovementTimeline.IsPlaying())
			{
				MovementTimeline.Play();
			}
			break;
		default:
			break;
	} //switch (RestartOption)
}

void AMovingSplineActor::TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	GEngine->AddOnScreenDebugMessage(400, 2.0f, FColor::Green, __FUNCTION__);
	switch (RestartOption)
	{
		case ASplineElevatorRestartOption::OFF:
		{
			if (!MovementTimeline.IsReversing())
			{
				MovementTimeline.Reverse();
			}
		} break;
		case ASplineElevatorRestartOption::DEFAULT:
			break;
		case ASplineElevatorRestartOption::FROM_BEGIN:
			break;
		case ASplineElevatorRestartOption::FROM_END:
			break;
		default:
			break;
	} // switch (RestartOption)

}

