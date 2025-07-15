// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/WarriorAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "WarriorDebugHelper.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "Net/UnrealNetwork.h"

UWarriorAttributeSet::UWarriorAttributeSet()
{
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentRage(1.f);
	InitMaxRage(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
}

void UWarriorAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/**
	 * 该宏允许游戏代码指定 RepNotify 条件：REPNOTIFY_OnChanged（默认）或 REPNOTIFY_Always （在调用 repnotify 函数时）
	 * Attribute 需要被添加到GetLifetimeReplicatedProps中，COND_None 为触发没有条件限制，
	 * REPTNOTIFY_Always 告诉 OnRep 方法在本地值和服务器下发的值即使已经相同也会触发（为了预测），默认情况下OnRep不会触发
	 * Meta Attribute不同步，不需要添加
	 */
	DOREPLIFETIME_CONDITION_NOTIFY(UWarriorAttributeSet, CurrentHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWarriorAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWarriorAttributeSet, CurrentRage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWarriorAttributeSet, MaxRage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWarriorAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWarriorAttributeSet, DefensePower, COND_None, REPNOTIFY_Always);
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
			// Handle Death：为Target的AvatarActor(注意)添加死亡状态标签
			UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(
				Data.Target.GetAvatarActor(),
				WarriorGameplayTags::Shared_Status_Dead
			);
		}
	}
}

void UWarriorAttributeSet::OnRep_CurrentHealth(const FGameplayAttributeData& OldCurrentHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWarriorAttributeSet, CurrentHealth, OldCurrentHealth);
}

void UWarriorAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWarriorAttributeSet, MaxHealth, OldMaxHealth);
}

void UWarriorAttributeSet::OnRep_CurrentRage(const FGameplayAttributeData& OldCurrentRage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWarriorAttributeSet, CurrentRage, OldCurrentRage);
}

void UWarriorAttributeSet::OnRep_MaxRage(const FGameplayAttributeData& OldMaxRage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWarriorAttributeSet, MaxRage, OldMaxRage);
}

void UWarriorAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWarriorAttributeSet, AttackPower, OldAttackPower);
}

void UWarriorAttributeSet::OnRep_DefensePower(const FGameplayAttributeData& OldDefensePower) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWarriorAttributeSet, DefensePower, OldDefensePower);
}
