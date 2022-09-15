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

// Attack type based on animaton montage
UENUM(BlueprintType)
enum class EAttackType : uint8 {
	MELEE_PUNCH			UMETA(DisplayName = "Punch attack"),
	MELEE_KICK    UMETA(DisplayName = "Kick attack")
};

// Attack type modificator based on animaton montage
UENUM(BlueprintType)
enum class EAttackModificator : uint8 {
	LIGHT			UMETA(DisplayName = "Light attack mod"),
	MEDIUM    UMETA(DisplayName = "Medium attack mod"),
	STRONG UMETA(DisplayName = "Strong attack mod")
};

UENUM(BlueprintType)
enum class ELineTraceType : uint8 {
	CAMERA_SINGLE				UMETA(DisplayName = "Single line, starts from camera"),
	PLAYER_SINGLE		UMETA(DisplayName = "Single line, starts from player"),
};

UCLASS(config=Game)
class AUE4FighterCharacter : public ACharacter
{
	GENERATED_BODY()

		/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	/** Holds all animation montage **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		UDataTable* PlayerMeleeAttackMontageDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundBase* PunchSoundCue;
	 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundBase* PunchThrowSoundCue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* LeftCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* RightCollisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = LineTrace, meta = (AllowPrivateAccess = "true"))
		ELineTraceType LineTraceType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = LineTrace, meta = (AllowPrivateAccess = "true"))
		float LineTraceDistance;
	
	/** Set how long to play OnArm animation idle after  player input hit */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		float OnArmedDelay;

public:
	AUE4FighterCharacter();

	// Called when the game starts or when the player is spawned
	virtual void BeginPlay() override;

	/**
	* Triggers attack animations based on user input
	*/
	void LightPunch();

	/**
	* Triggers attack animations based on user input
	*/
	void MediumPunch();

	/**
* Triggers attack animations based on user input
*/
	void StrongPunch();

	/**
	* Triggers attack animations based on user input
	*/
	void LightKick();

	/**
	* Triggers attack animations based on user input
	*/
	void MediumKick();

	/**
	* Triggers attack animations based on user input
	*/
	void StrongKick();

	/**
	* Triggers attack animations based on punch or kick
	*/
	void AttackInput(EAttackType AttackType, EAttackModificator AttackModificator);

	/**
	* Trigger arm animation after punch or kick
	*/
	void SetArmAnimationAfterHit();
	/**
	* Trigger off arm animation by timer 
	*/
	void ResetArmAnimationAfterHit();


	/** Sets crouching locomotion variable to proper state */
	void CrouchingLocomotionStart();

	/** Sets crouching locomotion variable to proper state */
	void CrouchingLocomotionEnd();

	/**
	* Initiates player attack by bool
	*/
	void SetPlayerMeleeCollision(bool bBoxCollision);

	/**
	* Trace lines from player camera or eyes
	*/
	void FireLineTrace();

	/**
	* Triggered when the collision hit event fires between our weapon and enemy entities
	*/
	UFUNCTION()
		void OnAttackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/**
	* Triggered when the collision overlap event begin
	*/
	UFUNCTION()
		void CollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	/**
	* Triggered when the collision overlap event ends
	*/
	UFUNCTION()
	void CollisionBoxEndOwerlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

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

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
	/* Check if animation is blended and return this value */
	UFUNCTION(BlueprintCallable, Category = Animation)
		bool GetIsAnimationBlended();

	/* Check if play arm animation */
	UFUNCTION(BlueprintCallable, Category = Animation)
		bool GetIsPlayerArm();

	/* Enable or disable player movement by bool */
	UFUNCTION(BlueprintCallable, Category = Animation)
		void SetPlayerMovement(bool PlayerMovement);

	/** Throw sound when punch animation starts */
 UAudioComponent* PunchThrowAudioComponent;

private:

	/** Trigger on attack hit when actor hit enemy */
	UAudioComponent* PunchAudioComponent;
	UAnimMontage* BaseAttackAnimationMontage;
	FMeleeCollisionProfile MeleeCollisionProfile;
	bool IsAnimationBlended;
	bool IsPlayerMovementEnable;
	bool IsArmed;
	// Reset combo count timer handler
	FTimerHandle ResetComboCountTimer;

	/** Count down arm/idle animation time */
	FTimerHandle StopArmAnimationTimer;

	int ComboCount;

	/** Resets combo counter after few seconds */
	void ResetCombo();
	/**
	* Log - prints a message to all the log outputs with a specific color
	* @param LogLevel {@see ELogLevel} affects color of log
	* @param FString the message for display
	*/
	void Log(ELogLevel LogLevel, FString Message);
	/**
	* Log - prints a message to all the log outputs with a specific color
	* @param LogLevel {@see ELogLevel} affects color of log
	* @param FString the message for display
	* @param ELogOutput - All, Output Log or Screen
	*/
	void Log(ELogLevel LogLevel, FString Message, ELogOutput LogOutput);
};

