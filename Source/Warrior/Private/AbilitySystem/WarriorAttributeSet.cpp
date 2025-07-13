// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/WarriorAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "WarriorDebugHelper.h"

UWarriorAttributeSet::UWarriorAttributeSet()
{
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentRage(1.f);
	InitMaxRage(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
}

void UWarriorAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetCurrentHealthAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.f, GetMaxHealth());
	}
	
	if (Attribute == GetCurrentRageAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.f, GetMaxRage());
	}
}

void UWarriorAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// 需要手动包含#include "GameplayEffectExtension.h"，以获取struct FGameplayEffectModCallbackData
	/**
	 * @note 造成即刻伤害/治疗属于Instant GE（关键是修改BaseValue），应该调用PostGameplayEffectExecute
	 * 并且像GameplayEffectExecutionCalculations和ModifierMagnitudeCalculations这种自所有Modifier重新计算CurrentValue的函数
	 * 需要**再次**执行限制(Clamp)操作.
	 */
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		// 对于GameplayEffectExecutionCalculation，需要再次进行Clamp操作
		const float NewCurrentHealth = FMath::Clamp<float>(GetCurrentHealth(), 0.f, GetMaxHealth());
		SetCurrentHealth(NewCurrentHealth);
	}

	if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
	{
		const float NewCurrentRage = FMath::Clamp<float>(GetCurrentRage(), 0.f, GetMaxRage());
		SetCurrentRage(NewCurrentRage);
	}

	// DamageTaken是占位符，属于Meta Attribute
	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();

		const float NewCurrentHealth = FMath::Clamp<float>(OldHealth - DamageDone, 0.f, GetMaxHealth());
		SetCurrentHealth(NewCurrentHealth);

		const FString& DebugString = FString::Printf(
			TEXT("\nOld Health: %f \nDamage Done: %f \nNew Current Health: %f \n"),
			OldHealth,
			DamageDone,
			NewCurrentHealth
		);
		Debug::Print(DebugString, FColor::Green);

		// todo: Notify the UI
		if (NewCurrentHealth == 0.f)
		{
			//todo: Handle Character Death
		}
	}
}
