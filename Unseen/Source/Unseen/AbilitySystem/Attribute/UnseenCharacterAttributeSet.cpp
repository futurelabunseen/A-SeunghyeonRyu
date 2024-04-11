// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attribute/UnseenCharacterAttributeSet.h"

UUnseenCharacterAttributeSet::UUnseenCharacterAttributeSet() :
	Stamina(100.0f),
	MaxStamina(100.0f),
	RollStaminaCost(-30.0f),
	StepBackStaminaCost(-30.0f),
	SprintStaminaCost(-1.0f)
{
	InitStamina(GetMaxStamina());
}

void UUnseenCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetStaminaAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("%f"), NewValue);
		
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
		
		UE_LOG(LogTemp, Warning, TEXT("%f"), NewValue);

		// ������ ����� â���� 100���� ���´�.
		// �׷��� ��°��?
		// Base Value�� Current Value�� �������ִ�.
		// �̰� ���� �����ε�
		// Attribute set������ GetCurrentValue�� �ٲٴµ�.
	}
	Super::PreAttributeChange(Attribute, NewValue);
}

void UUnseenCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}
