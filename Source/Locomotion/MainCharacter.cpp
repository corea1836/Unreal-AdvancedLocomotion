#include "MainCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	
	CameraBoom->TargetArmLength = 350.0f;
	CameraBoom->bUsePawnControlRotation = true;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	
	Camera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false; 
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	

}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMainCharacter::MoveForward(float Value)
{
	if (GetController() == nullptr && Value == 0.0f) return;
	
	const FRotator Rotation = GetController()->GetControlRotation();
	
	const FRotator YawRotation { 0.0f, Rotation.Yaw, 0.0f };
	
	const FVector ForwardDirection { FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) };
	
	AddMovementInput(ForwardDirection, Value);
	
}

void AMainCharacter::MoveRight(float Value)
{
	if (GetController() == nullptr && Value == 0.0f) return;
	
	const FRotator Rotation = GetController()->GetControlRotation();
	
	const FRotator YawRotation { 0.0f, Rotation.Yaw, 0.0f };
	
	const FVector RightDirection { FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) };
	
	AddMovementInput(RightDirection, Value);
}

void AMainCharacter::Turn(float Value)
{
	if (GetController() == nullptr && Value == 0.0f) return;
	
	AddControllerYawInput(Value);
}

void AMainCharacter::LookUp(float Value)
{
	if (GetController() == nullptr && Value == 0.0f) return;
	
	AddControllerPitchInput(Value);
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	check(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ThisClass::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ThisClass::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ThisClass::LookUp);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ThisClass::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ThisClass::StopJumping);
}

