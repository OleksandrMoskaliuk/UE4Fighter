// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"

#include "Engine/DataTable.h"

#include "UE4FighterCharacter.generated.h"

/** With this data structure we can take animation montage wit section count to play */
USTRUCT(BlueprintType)
struct FPlayerMeleeAttackMontageDataTable : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UAnimMontage* Montage;

	/** How many animation have current montage **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 SectionCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Description;
};

USTRUCT(BlueprintType)
struct FMeleeCollisionProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName Enabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName Disabled;

	// default constructor
	FMeleeCollisionProfile() {
		Enabled = FName(TEXT("Weapon"));
		Disabled = UCollisionProfile::NoCollision_ProfileName;
	}
	/*UCollisionProfile::NoCollision_ProfileName = FName(TEXT("NoCollision"));
	UCollisionProfile::BlockAll_ProfileName = FName(TEXT("BlockAll"));
	UCollisionProfile::PhysicsActor_ProfileName = FName(TEXT("PhysicsActor"));
	UCollisionProfile::BlockAllDynamic_ProfileName = FName(TEXT("BlockAllDynamic"));
	UCollisionProfile::Pawn_ProfileName = FName(TEXT("Pawn"));
	UCollisionProfile::Vehicle_ProfileName = FName(TEXT("Vehicle"));
	UCollisionProfile::DefaultProjectile_ProfileName = FName(TEXT("DefaultProjectile"));*/
};


UENUM(BlueprintType)
enum class ELogLevel : uint8 {
	TRACE			UMETA(DisplayName = "Trace"),
	DEBUG			UMETA(DisplayName = "Debug"),
	INFO			UMETA(DisplayName = "Info"),
	WARNING			UMETA(DisplayName = "Warning"),
	ERROR			UMETA(DisplayName = "Error")
};

UENUM(BlueprintType)
enum class ELogOutput : uint8 {
	ALL				UMETA(DisplayName = "All levels"),
	OUTPUT_LOG		UMETA(DisplayName = "Output log"),
	SCREEN			UMETA(DisplayName = "Screen")
};

UENUM(BlueprintType)
enum class EAttackType : uint8 {
	MELEE_FIST			UMETA(DisplayName = "Melee - Fist")
};

UCLASS(config=Game)
class AUE4FighterCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AUE4FighterCharacter();

	// Called when the game starts or when the player is spawned
	virtual void BeginPlay() override;

	/**
	* Triggers attack animations based on user input
	*/
	void AttackInput();

	/**
	* Initiates player attack by bool
	*/
	void SetPlayerMeleeCollision(bool bBoxCollision);

	/**
	* Triggered when the collision hit event fires between our weapon and enemy entities
	*/
	UFUNCTION()
		void OnAttackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Holds all animation montage **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		UDataTable* PlayerMeleeAttackMontageDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundBase* PunchSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundBase* PunchThrowSoundCue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* LeftFistCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* RightFistCollisionBox;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	UAudioComponent* PunchThrowAudioComponent;
private:
	UAudioComponent* PunchAudioComponent;
	/** Holds all collision profiles **/
	FMeleeCollisionProfile MeleeCollisionProfile;
	/**
	* Log - prints a message to all the log outputs with a specific color
	* @param LogLevel {@see ELogLevel} affects color of log
	* @param FString the message for display
	*/
	void Log(ELogLevel LogLevel1, FString Message);
	/**
	* Log - prints a message to all the log outputs with a specific color
	* @param LogLevel {@see ELogLevel} affects color of log
	* @param FString the message for display
	* @param ELogOutput - All, Output Log or Screen
	*/
	void Log(ELogLevel LogLevel1, FString Message, ELogOutput LogOutput);
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


};

