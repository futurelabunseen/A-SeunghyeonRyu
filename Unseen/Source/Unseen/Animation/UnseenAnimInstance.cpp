// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/UnseenAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"


UUnseenAnimInstance::UUnseenAnimInstance()
{
	MovingThreshold = 3.0f;
	JumpingThreshold = 100.0f;
	
}

void UUnseenAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<AUnseenCharacterPlayer>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}



void UUnseenAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	//AUnseenCharacterPlayer* UnseenCharacter = CastChecked<AUnseenCharacterPlayer>(ActorInfo->AvatarActor.Get());
	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshold;
		bIsFalling = Movement->IsFalling();
		//UE_LOG(LogTemp, Warning, TEXT("bIsFalling : %u"), bIsFalling);
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshold);
		//UE_LOG(LogTemp, Warning, TEXT("bIsJumping : %u"), bIsJumping);
		Direction = CalculateDirection(Velocity, Owner->GetActorRotation());
		bIsAiming = Owner->bIsAiming;

		if (bIsAiming)
		{
			Pitch = Owner->GetBaseAimRotation().Pitch;
			if (Pitch >= 180) { Pitch -= 360.f; }
		}
		else { Pitch = 0; }
	}
}

