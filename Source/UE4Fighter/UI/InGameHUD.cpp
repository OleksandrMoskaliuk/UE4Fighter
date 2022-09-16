// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/InGameHUD.h"

AInGameHUD::AInGameHUD() {
}

void AInGameHUD::DrawHUD() {
 AHUD::DrawHUD();

}

void AInGameHUD::BeginPlay() {
 Super::BeginPlay();

 if (ComboWidgetClass) 
 {
  ComboWidget = CreateWidget<UComboWidget>(GetWorld(), ComboWidgetClass,FName("Combo Widget"));
  if (ComboWidget) 
  {
   ComboWidget->AddToViewport();
  }
 }

}

void AInGameHUD::Tick(float DeltaSeconds) {
 Super::Tick(DeltaSeconds);
}

void AInGameHUD::UpdateComboCount(int32 ComboCount) {
 if (ComboWidget) 
 {
  ComboWidget->UpdateComboCount(ComboCount);
 }
}

void AInGameHUD::ResetCombo() {
 if (ComboWidget) 
 {
  ComboWidget->ResetCombo();
 }
}
