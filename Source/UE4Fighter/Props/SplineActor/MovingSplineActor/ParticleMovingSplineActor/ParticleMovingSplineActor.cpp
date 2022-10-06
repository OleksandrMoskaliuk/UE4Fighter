// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleMovingSplineActor.h"

AParticleMovingSplineActor::AParticleMovingSplineActor() {
 ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle component"));
 ParticleComponent->SetupAttachment(AMovingSplineActor::SplineComponent);

}

void AParticleMovingSplineActor::Tick(float DeltaTime) {
 AMovingSplineActor::Tick(DeltaTime);
}

void AParticleMovingSplineActor::BeginPlay() {
 AMovingSplineActor::BeginPlay();

 //ParticleComponent->SetAutoActivate(true);
 // Such as auto activate spline option 
 if (AMovingSplineActor::SplineComponent->IsActive())
 {
  ParticleComponent->Activate();
 }
 else 
 {
  ParticleComponent->Deactivate();
 }
}

void AParticleMovingSplineActor::StartProcessMovementTimeline(float MovementCurveValue) {
 AMovingSplineActor::StartProcessMovementTimeline(MovementCurveValue);
 FVector ParticleComponentLocation = AMovingSplineActor::GetSplineLocation();
 FRotator ParticleComponentRotation = AMovingSplineActor::GetSplineRotation();

 ParticleComponent->SetWorldLocationAndRotation(ParticleComponentLocation, ParticleComponentRotation);

 if (MovementCurveValue <= 0.001f)
 {
  ParticleComponent->Deactivate();
 }
 else{
  ParticleComponent->Activate();
 }
}

void AParticleMovingSplineActor::TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
 AMovingSplineActor::TriggerBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
 if (AMovingSplineActor::RestartOption == ASplineElevatorRestartOption::OFF && !ParticleComponent->IsActive() ) 
 {
  ParticleComponent->Activate();
 }
 
}

void AParticleMovingSplineActor::TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
 AMovingSplineActor::TriggerEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
 
}
