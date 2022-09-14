// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/ComboWidget.h"
#include "Animation/WidgetAnimation.h"


UComboWidget::UComboWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer) {
}

void UComboWidget::NativeConstruct() {
 Super::NativeConstruct();
	this->StoreWidgetAnimations();

	ComboFaadeAnimation = StoreWidgetAnimationByName(TEXT("ComboFade"));
	ComboShakeAnimation = StoreWidgetAnimationByName(TEXT("ComboShake"));
}

void UComboWidget::UpdateComboCount(int32 ComboCount) {
 if (TXTCombo && ComboCount > 1) 
 {
  if (!TXTCombo->IsVisible()) 
  {
   TXTCombo->SetVisibility(ESlateVisibility::Visible);
  }
  TXTCombo->SetText(FText::FromString(FString::FromInt(ComboCount) + "x Combo"));
 }

	if (ComboFaadeAnimation) 
	{
		PlayAnimation(ComboFaadeAnimation, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
	}

	if (ComboShakeAnimation)
	{
		PlayAnimation(ComboShakeAnimation, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
	}

}

void UComboWidget::ResetCombo() {
 if (TXTCombo) 
 {
  TXTCombo->SetVisibility(ESlateVisibility::Hidden);
 }
}

void UComboWidget::StoreWidgetAnimations() {
	AnimationsMap.Empty();
	UProperty* Prop = GetClass()->PropertyLink;
	// check each property of this class
	while (Prop)
	{
		// only evaluate object properties, skip rest
		if (Prop->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* ObjProp = Cast<UObjectProperty>(Prop);

			// only get back properties that are of type widget animation
			if (ObjProp->PropertyClass == UWidgetAnimation::StaticClass())
			{
				UObject* Obj = ObjProp->GetObjectPropertyValue_InContainer(this);
				// only get back properties that are of type widget animation
				UWidgetAnimation* WidgetAnimation = Cast<UWidgetAnimation>(Obj);
				// if casting worked update map with new animation
				if (WidgetAnimation && WidgetAnimation->MovieScene)
				{
					FName AnimName = WidgetAnimation->MovieScene->GetFName();
					GEngine->AddOnScreenDebugMessage(-1, 4.5f, FColor::Magenta, AnimName.ToString());
					AnimationsMap.Add(AnimName, WidgetAnimation);
				}
			}
		}
		Prop = Prop->PropertyLinkNext;
	}
}

UWidgetAnimation* UComboWidget::StoreWidgetAnimationByName(FName AnimationName) const {

	UWidgetAnimation* const* WidgetAnimation = AnimationsMap.Find(AnimationName);
	if (WidgetAnimation) 
	{
		return *WidgetAnimation;
	}
	return nullptr;



}
