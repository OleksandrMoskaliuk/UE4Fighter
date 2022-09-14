// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Runtime/UMG/Public/UMG.h"

#include "ComboWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE4FIGHTER_API UComboWidget : public UUserWidget
{

	GENERATED_BODY()

public:
 UComboWidget(const FObjectInitializer& ObjectInitializer);

 virtual void NativeConstruct() override;

 void UpdateComboCount(int32 ComboCount);

 void ResetCombo();

 UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
  class UTextBlock* TXTCombo;

 /** This method will collect all animation at start */
 void StoreWidgetAnimations();

 UWidgetAnimation* StoreWidgetAnimationByName(FName AnimationName) const;

private:

 TMap<FName, UWidgetAnimation*> AnimationsMap;

 class UWidgetAnimation* ComboFaadeAnimation;

 class UWidgetAnimation* ComboShakeAnimation;

};
