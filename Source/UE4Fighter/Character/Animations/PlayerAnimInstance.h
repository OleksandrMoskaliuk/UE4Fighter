// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE4FIGHTER_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Instance")
  bool IsInAir;

 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Instance")
  bool IsAnimationBlended;

 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Instance")
  float Speed;

 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Instance")
  bool IsCrouching;

 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Instance")
  bool IsArmed;

public:

 UPlayerAnimInstance();

 virtual void NativeInitializeAnimation() override;

 virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
 APawn* Owner;
};
