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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	/** Holds all animation montage **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UDataTable* PlayerMeleeAttackMontageDataTable;

	/** Counter for  geting back to idle animation **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		float MaxCountDownToIdle;

	/** Material, for set possesed back to normal, body  **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Materials", meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* DefaultMaterialBody;

	/** Material, for set possesed back to normal, chest  **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Materials", meta = (AllowPrivateAccess = "true"))
		UMaterialInstance* DefaultMaterialChest;

	/** Material, for current character after possessing, body **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Materials", meta = (AllowPrivateAccess = "true"))
		UMaterialInstance* PossesedMaterialBody;

	/** Material, for current character after possessing, chest  **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Materials", meta = (AllowPrivateAccess = "true"))
		UMaterialInstance* PossesedMaterialChest;

	//** Movement state management */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State Management", meta = (AllowPrivateAccess = "true"))
		bool bIsWalking;

	//** Movement state management */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State Management", meta = (AllowPrivateAccess = "true"))
	 bool IsAnimationBlended;

	//** Movement state management */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State Management", meta = (AllowPrivateAccess = "true"))
	 bool bIsPlayerMovementEnable;

	//** Movement state management */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State Management", meta = (AllowPrivateAccess = "true"))
	 bool bIsArmed;

	/** Audio component, play when on hit event happens */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
		class USoundBase* PunchSoundCue;

	/** Audio component, play when player press attack button */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
		class USoundBase* PunchThrowSoundCue;

	/** Collision, LeftCollisionBox */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* LeftCollisionBox;

	/** Collision, Right CollisionBox */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* RightCollisionBox;

	/**Line trace, type that decide to strike from player eyes or head */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LineTrace", meta = (AllowPrivateAccess = "true"))
		ELineTraceType LineTraceType;

	/**Line trace, distance between start & end points */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LineTrace", meta = (AllowPrivateAccess = "true"))
		float LineTraceDistance;

	/**Line trace, offset to shift start point next to end point  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LineTrace", meta = (AllowPrivateAccess = "true"))
		float LineTraceStartPointOffsetDistance;
	
	/**Line trace, decides whether character possessed or not */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LineTrace", meta = (AllowPrivateAccess = "true"))
	 bool bIsCurrentlyPossessed;

	/** Animation,  speed when player in run state  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		float PlayerRunSpeed;

	/** Animation, speed when player in walk state */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		float PlayerWalkSpeed;

	/** Animation, speed when player in armed state */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		float PlayerOnArmedlSpeed;

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

	/**
	* Switch between walk and run animation 
	* basically change player speed
	*/
	void SwitchPlayerWalkRunAnimation();

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
	* possess another player character by line trace
	*/
	void PossessAnotherCharacter();

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

	/** Throw sound when punch animation starts */
	UAudioComponent* PunchThrowAudioComponent;

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

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public: // Getters and Setters

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

	/* Check if play arm animation */
	UFUNCTION(BlueprintCallable, Category = "Animation")
		bool GetIsPlayerWalking();

	/* Enable or disable player movement by bool */
	UFUNCTION(BlueprintCallable, Category = Animation)
		void SetPlayerMovement(bool PlayerMovement);

	/* Check if animation is blended and return this value */
	UFUNCTION(BlueprintCallable, Category = "Locomotion")
		float GetMoveForwardValue();

	/* Check if animation is blended and return this value */
	UFUNCTION(BlueprintCallable, Category = "Locomotion")
		float GetMoveRightValue();

private:

	/** Trigger on attack hit when actor hit enemy */
	UAudioComponent* PunchAudioComponent;

	/** Reset combo count timer handler */
	FTimerHandle ResetComboCountTimer;

	/** Count down arm/idle animation time */
	FTimerHandle StopArmAnimationTimer;

	/** Saved controller, used for possessing */
	AController* SavedController;

	UAnimMontage* BaseAttackAnimationMontage;
	FMeleeCollisionProfile MeleeCollisionProfile;

	//Inputs from controller 
	float MoveForwardValue;
	float MoveRightValue;

	// Combo hit counter
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

