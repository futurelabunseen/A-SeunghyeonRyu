// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_Roll.h"
#include "Character/UnseenCharacterPlayer.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attribute/UnseenCharacterAttributeSet.h"
#include "AbilitySystem/Effect/GE_RollStamina.h"

UGA_Roll::UGA_Roll()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Character.Action.Roll"));

	ActivationRequiredTags.AddTag(FGameplayTag::RequestGameplayTag("Character.State.IsMoving"));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag("Character.State.IsRolling"));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Character.Action.Move"));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("Character.Action.Aim"));

	RollStaminaEffect = UGE_RollStamina::StaticClass();
}

void UGA_Roll::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AUnseenCharacterPlayer* UnseenCharacter = CastChecked<AUnseenCharacterPlayer>(ActorInfo->AvatarActor.Get());
	UnseenCharacter->SetActorRotation(FRotationMatrix::MakeFromX(UnseenCharacter->GetLastMovementInputVector()).Rotator());
	
	// Use Stamina ���� �Լ��� ���� �� ��
	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(RollStaminaEffect);
	if (EffectSpecHandle.IsValid())
	{
		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle);
	}

	UnseenCharacter->bIsRollStepBackActive = true;
	UnseenCharacter->RollStepBackCameraLerp();
	UAbilityTask_PlayMontageAndWait* PlayRollTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayRoll"), UnseenCharacter->GetRollMontage());

	UnseenCharacter->GetRollMovementTimeline()->PlayFromStart();

	PlayRollTask->OnCompleted.AddDynamic(this, &UGA_Roll::OnCompleteCallback);
	PlayRollTask->OnInterrupted.AddDynamic(this, &UGA_Roll::OnInterruptedCallback);
	PlayRollTask->ReadyForActivation();
	
}

bool UGA_Roll::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	// ���׹̳� �˻�
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo_Checked();
	const UUnseenCharacterAttributeSet* AttributeSet = ASC->GetSet<UUnseenCharacterAttributeSet>();
	if (AttributeSet->GetStamina() < 30.0f) // �̰� 30.0f�� GE_RollStamina�� cost �������� ��� ������ ����� const�Լ�
	{
		return false;
	}

	return true;
}

void UGA_Roll::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AUnseenCharacterPlayer* UnseenCharacter = CastChecked<AUnseenCharacterPlayer>(ActorInfo->AvatarActor.Get());
	UnseenCharacter->bIsRollStepBackActive = false;
	UnseenCharacter->RollStepBackCameraLerp();
	UnseenCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

}

void UGA_Roll::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UGA_Roll::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}