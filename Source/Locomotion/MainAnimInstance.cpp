#include "MainAnimInstance.h"
#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMainAnimInstance::NativeInitializeAnimation()
{
	MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
	
	CharacterRotationYaw = 0;
	LastCharacterRotationYaw = 0;
	RootRotationYaw = 0;
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

		if (MainCharacter->GetVelocity().Size() > 0.0f)
		{
			LastMovementYawDelta = MovementYawDelta;
		}
		
		if (Speed > 0)
		{
			RootRotationYaw = 0;
			CharacterRotationYaw = MainCharacter->GetActorRotation().Yaw;
			LastCharacterRotationYaw = CharacterRotationYaw;
			
			LastTurningCurve = 0;
			TurningCurve = 0;
		} 
		else
		{
			// Turn-in Place
			LastCharacterRotationYaw = CharacterRotationYaw;
		
			CharacterRotationYaw = MainCharacter->GetActorRotation().Yaw;
		
			const float CharacterRotationYawDelta = CharacterRotationYaw - LastCharacterRotationYaw;
		
			RootRotationYaw = UKismetMathLibrary::NormalizeAxis(RootRotationYaw - CharacterRotationYawDelta);
		
			IsTurning = GetCurveValue(FName(TEXT("IsTurning")));
		
			if (IsTurning > 0)
			{
				LastTurningCurve = TurningCurve;
				TurningCurve = GetCurveValue(FName(TEXT("Turning")));
			
				const float TurningCurveDelta = TurningCurve - LastTurningCurve;
			
				RootRotationYaw > 0 ? RootRotationYaw -= TurningCurveDelta : RootRotationYaw += TurningCurveDelta;
			
				if (FMath::Abs(RootRotationYaw) > 90.0f)
				{
					const float AdditionalRootRotationYaw = FMath::Abs(RootRotationYaw) - 90.0f;
				
					RootRotationYaw > 0 ? RootRotationYaw -= AdditionalRootRotationYaw :
					RootRotationYaw += AdditionalRootRotationYaw;
				}
			}
		}
	}
}
