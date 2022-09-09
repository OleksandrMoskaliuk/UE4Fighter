// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackAnimNotifyState.h"
#include "../Character/UE4FighterCharacter.h"

void UAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {
 //GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Cyan, __FUNCTION__);
 if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
 {
  AUE4FighterCharacter* Player = Cast<AUE4FighterCharacter>(MeshComp->GetOwner());
  if (Player != NULL)
  {
   Player->SetPlayerMeleeCollision(true);
  }
 }
}

void UAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
 //GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Cyan, __FUNCTION__);
 if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
 {
  AUE4FighterCharacter* Player = Cast<AUE4FighterCharacter>(MeshComp->GetOwner());
  if (Player != NULL)
  {
   Player->SetPlayerMeleeCollision(false);
  }
 }
}
