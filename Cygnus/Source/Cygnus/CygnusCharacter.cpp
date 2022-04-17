#include "CygnusCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

ACygnusCharacter::ACygnusCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;	
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	UE_LOG(LogTemp, Warning, TEXT("Construct"));
}

void ACygnusCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Begin playing"));

	CygnusGameInstance = GetWorld()->GetGameInstance<UCygnusGameInstance>();

	if (CygnusGameInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameInstance not instantiated, cannot instantiate FirebaseObject and Inventory"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Should be instantiated"));

		FirebaseObject = CygnusGameInstance->FirebaseObject;
		Inventory = CygnusGameInstance->Inventory;

		CygnusGameInstance->OnInventoryInstantiated.Broadcast(true);
	}

	discord::Core::Create(855988240447832065, DiscordCreateFlags_NoRequireDiscord, &DiscordCore);

	if (!CygnusGameInstance->DiscordActivitySet)
	{
		UE_LOG(LogTemp, Warning, TEXT("Setting DiscordActivity..."));
		
		DiscordActivity.GetAssets().SetLargeImage("cygnus");
		DiscordActivity.GetAssets().SetLargeText("Cygnus");
	
		DiscordActivity.GetAssets().SetSmallImage("alientech");
		DiscordActivity.GetAssets().SetSmallText("Made by AlienTech");
	
		DiscordActivity.SetDetails("Developing Cygnus - Coming Soon!");
		DiscordActivity.SetState("Coding Solo");

		DiscordActivity.GetTimestamps().SetStart(FDateTime().Now().ToUnixTimestamp() - GetWorld()->GetRealTimeSeconds());

		DiscordActivity.GetParty().SetId("2004083120050519");
		DiscordActivity.GetParty().GetSize().SetCurrentSize(1);
		DiscordActivity.GetParty().GetSize().SetMaxSize(4);

		DiscordActivity.GetSecrets().SetJoin("831");
		DiscordActivity.GetSecrets().SetSpectate("519");

		DiscordActivity.SetType(discord::ActivityType::Playing);
	
		DiscordCore->ActivityManager().UpdateActivity(DiscordActivity, [](discord::Result Result)
		{
			UE_LOG(LogTemp, Warning, TEXT("Finished setting DiscordActivity"));
		});
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Skipped setting DiscordActivity"));
	}
}

void ACygnusCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	DiscordCore->RunCallbacks();
}

void ACygnusCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACygnusCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACygnusCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACygnusCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACygnusCharacter::LookUpAtRate);
	
	PlayerInputComponent->BindAction("ToggleInventory", IE_Pressed, this, &ACygnusCharacter::ToggleInventoryCall);

	for (int n = 1; n <= 10; n++)
	{
		PlayerInputComponent->BindAction(FName(TEXT("Hotbar%d"), n), IE_Pressed, this, &ACygnusCharacter::ToggleInventoryCall);
	}
}

void ACygnusCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACygnusCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACygnusCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACygnusCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ACygnusCharacter::ToggleInventoryCall()
{
	CygnusGameInstance->ToggleInventory();
}