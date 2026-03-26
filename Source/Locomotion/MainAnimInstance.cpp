#include "MainAnimInstance.h"
#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMainAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
}

void UMainAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (MainCharacter)
	{
		FVector Velocity = MainCharacter->GetVelocity();
		Velocity.Z = 0;

		Speed = Velocity.Size();

		bIsAccelerating = MainCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f;

		bIsInAir = MainCharacter->GetCharacterMovement()->IsFalling();
		
		FRotator BaseAnimRotation = MainCharacter->GetBaseAimRotation();
		
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(MainCharacter->GetVelocity());
		
		MovementYawDelta = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, BaseAnimRotation).Yaw;
	}
}
