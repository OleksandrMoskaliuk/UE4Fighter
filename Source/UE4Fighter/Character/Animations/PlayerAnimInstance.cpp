// Fill out your copyright notice in the Description page of Project Settings.


#include "../Animations/PlayerAnimInstance.h"
#include "../../Character/UE4FighterCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Engine.h"

UPlayerAnimInstance::UPlayerAnimInstance() {
 IsInAir = false;
 IsAnimationBlended = false;
 Speed = 0.f;
}

void UPlayerAnimInstance::NativeInitializeAnimation() {
 Super::NativeInitializeAnimation();
 //cache pawn owner for later use
 Owner = this->TryGetPawnOwner();
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
 Super::NativeUpdateAnimation(DeltaSeconds);
 if(!Owner)
 {
  return;
 }

 if (Owner->IsA(AUE4FighterCharacter::StaticClass())) 
 {
  AUE4FighterCharacter* PlayerCharacter = Cast<AUE4FighterCharacter>(Owner);
  if (PlayerCharacter) 
  {
   IsInAir = PlayerCharacter->GetMovementComponent()->IsFalling();
   IsAnimationBlended = PlayerCharacter->GetIsAnimationBlended();
   Speed = PlayerCharacter->GetVelocity().Size();
   IsCrouching = PlayerCharacter->GetMovementComponent()->IsCrouching();
   GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "IsCrouching: " + FString(IsCrouching ? "true" : "false"));
   //GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "IsInAir: " + FString(IsInAir ? "true" : "false"));
   //GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "Is Animation Blended: " + FString(IsAnimationBlended ? "true" : "false"));
   //GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "Speed: " + FString::SanitizeFloat(Speed));
  }
 }
}
