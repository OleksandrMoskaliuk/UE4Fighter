// Fill out your copyright notice in the Description page of Project Settings.

#include "BlockMovementNotifyState.h"
#include "../Character/UE4FighterCharacter.h"

void UBlockMovementNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {
}

void UBlockMovementNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) {
 if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
 {
  AUE4FighterCharacter* Player = Cast<AUE4FighterCharacter>(MeshComp->GetOwner());
  if (Player != NULL)
  {
   Player->SetPlayerMovement(false);
  }
 }
}

void UBlockMovementNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
 if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
 {
  AUE4FighterCharacter* Player = Cast<AUE4FighterCharacter>(MeshComp->GetOwner());
  if (Player != NULL)
  {
   Player->SetPlayerMovement(true);
  }
 }
}
