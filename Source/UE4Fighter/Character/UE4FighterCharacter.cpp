// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE4FighterCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// AUE4FighterCharacter

AUE4FighterCharacter::AUE4FighterCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// load the sound cue object
	static ConstructorHelpers::FObjectFinder<USoundBase> PunchSoundCueObject(TEXT("SoundCue'/Game/Mannequin/Sounds/Effects/punch_01_Cue.punch_01_Cue'"));
	if (PunchSoundCueObject.Succeeded())
	{
		PunchSoundCue = PunchSoundCueObject.Object;

		PunchAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PunchAudioComponent"));
		PunchAudioComponent->SetupAttachment(RootComponent);
	}

	// load the punch throw sound cue object
	static ConstructorHelpers::FObjectFinder<USoundBase> PunchThrowSoundCueObject(TEXT("SoundCue'/Game/Mannequin/Sounds/Effects/medium_punch_throw_01_Cue.medium_punch_throw_01_Cue'"));
	if (PunchThrowSoundCueObject.Succeeded())
	{
		PunchThrowSoundCue = PunchThrowSoundCueObject.Object;

		PunchThrowAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PunchThrowAudioComponent"));
		PunchThrowAudioComponent->SetupAttachment(RootComponent);
	}

		// load the attack montage data table
	static ConstructorHelpers::FObjectFinder<UDataTable> AttackMontageDataTable(TEXT("DataTable'/Game/Mannequin/Animations/AnimationMontages/MainAttacks.MainAttacks'"));
	if (AttackMontageDataTable.Succeeded())
	{
		PlayerMeleeAttackMontageDataTable = AttackMontageDataTable.Object;
	}

	LeftFistCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftFistCollisionBox"));
	LeftFistCollisionBox->SetupAttachment(RootComponent);
	LeftFistCollisionBox->SetCollisionProfileName(MeleeCollisionProfile.Disabled);
	LeftFistCollisionBox->SetNotifyRigidBodyCollision(false);

	LeftFistCollisionBox->SetHiddenInGame(false);

	RightFistCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RightFistCollisionBox"));
	RightFistCollisionBox->SetupAttachment(RootComponent);
	RightFistCollisionBox->SetCollisionProfileName(MeleeCollisionProfile.Disabled);
	RightFistCollisionBox->SetNotifyRigidBodyCollision(false);

	RightFistCollisionBox->SetHiddenInGame(false);


}

void AUE4FighterCharacter::BeginPlay() {

	Super::BeginPlay();

	// attach collision components to sockets based on transformations definitions
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	LeftFistCollisionBox->AttachToComponent(GetMesh(), AttachmentRules, "fist_l_collision");
	RightFistCollisionBox->AttachToComponent(GetMesh(), AttachmentRules, "fist_r_collision");

	LeftFistCollisionBox->OnComponentHit.AddDynamic(this, &AUE4FighterCharacter::OnAttackHit);
	RightFistCollisionBox->OnComponentHit.AddDynamic(this, &AUE4FighterCharacter::OnAttackHit);

	LeftFistCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AUE4FighterCharacter::CollisionBoxBeginOverlap);
	RightFistCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AUE4FighterCharacter::CollisionBoxBeginOverlap);

	LeftFistCollisionBox->OnComponentEndOverlap.AddDynamic(this, &AUE4FighterCharacter::CollisionBoxEndOwerlap);
	RightFistCollisionBox->OnComponentEndOverlap.AddDynamic(this, &AUE4FighterCharacter::CollisionBoxEndOwerlap);

	// make sure our audio variables are initialized
	if (PunchSoundCue && PunchAudioComponent)
	{
		// attach the sound cue to our audio component 
		// NOTE: do not do this in the constructor as it will play the sound when the player spawns
		PunchAudioComponent->SetSound(PunchSoundCue);
	}

	// make sure our audio variables are initialized
	if (PunchThrowSoundCue && PunchThrowAudioComponent)
	{
		// attach the sound cue to our audio component 
		// NOTE: do not do this in the constructor as it will play the sound when the player spawns
		PunchThrowAudioComponent->SetSound(PunchThrowSoundCue);
	}

	if (PlayerMeleeAttackMontageDataTable)
	{
		static const FString ContextStrring(TEXT("Player Attack context"));
		FPlayerMeleeAttackMontageDataTable* PlayerMontageStruct =
			PlayerMeleeAttackMontageDataTable->FindRow<FPlayerMeleeAttackMontageDataTable>(FName(TEXT("Selected")), ContextStrring, true);
		if (PlayerMontageStruct)
		{
			BaseAttackAnimationMontage = PlayerMontageStruct->Montage;
			AnimationMontageSectionCount = PlayerMontageStruct->SectionCount;
		}
	}

}

void AUE4FighterCharacter::AttackInput() {
	// play throw punch sound if exist
	if (PunchThrowAudioComponent && !PunchThrowAudioComponent->IsPlaying())
	{
		PunchThrowAudioComponent->Play(0.f);
	}

	if (BaseAttackAnimationMontage)
	{
		// generate  number between 1 and 3:
		int MontageSectionIndex = FMath::FRandRange(1, AnimationMontageSectionCount);
		// create a montage section
		FString MontageSection = "start_" + FString::FromInt(MontageSectionIndex);
  PlayAnimMontage(BaseAttackAnimationMontage, 1.f, FName(*MontageSection));
	}

}

void AUE4FighterCharacter::SetPlayerMeleeCollision(bool bBoxCollision) {

	FName NewCollisionProfileName;
	bBoxCollision ? NewCollisionProfileName = MeleeCollisionProfile.Enabled : NewCollisionProfileName = MeleeCollisionProfile.Disabled;

	LeftFistCollisionBox->SetCollisionProfileName(NewCollisionProfileName);
	LeftFistCollisionBox->SetNotifyRigidBodyCollision(bBoxCollision);

	RightFistCollisionBox->SetCollisionProfileName(NewCollisionProfileName);
	RightFistCollisionBox->SetNotifyRigidBodyCollision(bBoxCollision);
}

void AUE4FighterCharacter::OnAttackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	Log(ELogLevel::DEBUG, __FUNCTION__);
	if(PunchAudioComponent && !PunchAudioComponent->IsPlaying())
	{
		PunchAudioComponent->SetPitchMultiplier(FMath::RandRange(1.f, 3.f));
	 PunchAudioComponent->Play(0.f);
	}

	UAnimInstance* AnimInstance = (GetMesh()) ? GetMesh()->GetAnimInstance() : nullptr;
	if (AnimInstance && BaseAttackAnimationMontage)
	{
		
			//kind of works but still go through animation
			//AnimInstance->Montage_Stop(1.5f, PlayerMontageStruct->Montage);

			AnimInstance->Montage_Pause(BaseAttackAnimationMontage);
			float last_pose_time = AnimInstance->Montage_GetPosition(BaseAttackAnimationMontage);
			AnimInstance->Montage_Play(BaseAttackAnimationMontage, -1.2f, EMontagePlayReturnType::Duration, last_pose_time,true);
		
	}
}


void AUE4FighterCharacter::CollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Log(ELogLevel::INFO, __FUNCTION__);
}

void AUE4FighterCharacter::CollisionBoxEndOwerlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Log(ELogLevel::INFO, __FUNCTION__);
}


// Input
void AUE4FighterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AUE4FighterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUE4FighterCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AUE4FighterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AUE4FighterCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AUE4FighterCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AUE4FighterCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AUE4FighterCharacter::OnResetVR);
	// main attack input functionality
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AUE4FighterCharacter::AttackInput);
}

void AUE4FighterCharacter::Log(ELogLevel LogLevel1, FString Message) {
	Log(LogLevel1, Message, ELogOutput::ALL);
}

void AUE4FighterCharacter::Log(ELogLevel LogLevel1, FString Message, ELogOutput LogOutput) {

	// only print when screen is selected and the GEngine object is available
	if ((LogOutput == ELogOutput::ALL || LogOutput == ELogOutput::SCREEN) && GEngine)
	{
		// default color
		FColor LogColor = FColor::Cyan;
		// flip the color based on the type
		switch (LogLevel1)
		{
			case ELogLevel::TRACE:
				LogColor = FColor::Green;
				break;
			case ELogLevel::DEBUG:
				LogColor = FColor::Cyan;
				break;
			case ELogLevel::INFO:
				LogColor = FColor::White;
				break;
			case ELogLevel::WARNING:
				LogColor = FColor::Yellow;
				break;
			case ELogLevel::ERROR:
				LogColor = FColor::Red;
				break;
			default:
				break;
		}
		// print the message and leave it on screen ( 4.5f controls the duration )
		GEngine->AddOnScreenDebugMessage(-1, 4.5f, LogColor, Message);
	}

	if (LogOutput == ELogOutput::ALL || LogOutput == ELogOutput::OUTPUT_LOG)
	{
		// flip the message type based on error level
		switch (LogLevel1)
		{
			case ELogLevel::TRACE:
				UE_LOG(LogTemp, VeryVerbose, TEXT("%s"), *Message);
				break;
			case ELogLevel::DEBUG:
				UE_LOG(LogTemp, Verbose, TEXT("%s"), *Message);
				break;
			case ELogLevel::INFO:
				UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
				break;
			case ELogLevel::WARNING:
				UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
				break;
			case ELogLevel::ERROR:
				UE_LOG(LogTemp, Error, TEXT("%s"), *Message);
				break;
			default:
				UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
				break;
		}
	}
}

void AUE4FighterCharacter::OnResetVR()
{
	// If UE4Fighter is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in UE4Fighter.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AUE4FighterCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AUE4FighterCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AUE4FighterCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUE4FighterCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AUE4FighterCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AUE4FighterCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
