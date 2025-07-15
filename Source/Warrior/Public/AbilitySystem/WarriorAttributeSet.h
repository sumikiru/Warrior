// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "WarriorAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UWarriorAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * 修改发生前响应Attribute的CurrentValue(注意不是BaseValue)变化, 其是通过引用参数NewValue限制(Clamp)CurrentValue即将进行的修改的理想位置.
	 * 不用于游戏逻辑事件, 而主要在其中做限制操作。仅用于对CurrentValue进行Clamp操作
	 * @note 在这里做的任何限制都不会永久性地修改ASC中的Modifier, 只会修改查询Modifier的返回值, \n
	 * 这意味着像GameplayEffectExecutionCalculations和ModifierMagnitudeCalculations这种自所有Modifier重新计算CurrentValue的函数
	 * 需要**再次**执行限制(Clamp)操作.
	 */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	/**
	 * 仅在即刻(Instant/Periodic)GE对Attribute的**BaseValue**修改之后触发,当GE对其修改时, 这就是一个处理更多Attribute操作的有效位置.
	 * @note 注意是Instant GE，且修改的是BaseValue \n
	 * 当PostGameplayEffectExecute()被调用时, 对Attribute的修改已经发生, 但是还没有被同步回客户端,
	 * 因此在这里限制值不会造成对客户端的二次同步, 客户端只会接收到限制后的值.
	 */
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData &Data) override;

	// 生命值
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = "OnRep_CurrentHealth", Category = "Health")
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, CurrentHealth);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = "OnRep_MaxHealth", Category = "Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, MaxHealth);
	// 怒气值
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = "OnRep_CurrentRage", Category = "Rage")
	FGameplayAttributeData CurrentRage;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, CurrentRage);
	UPROPERTY(BlueprintReadOnly,  ReplicatedUsing = "OnRep_MaxRage",Category = "Rage")
	FGameplayAttributeData MaxRage;
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, MaxRage);
	// 伤害相关
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = "OnRep_AttackPower", Category = "Damage")
	FGameplayAttributeData AttackPower; //攻击力
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, AttackPower);
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = "OnRep_DefensePower", Category = "Damage")
	FGameplayAttributeData DefensePower; //防御力
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, DefensePower);
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DamageTaken; //受到的伤害值，Meta Attribute一般是不可同步的
	ATTRIBUTE_ACCESSORS(UWarriorAttributeSet, DamageTaken);

	// 设置每个Attribute对应的服务器同步函数。OnRep函数需要调用GAMEPLAYATTRIBUTE_REPNOTIFY宏才能使用预测系统
	UFUNCTION()
	void OnRep_CurrentHealth(const FGameplayAttributeData& OldCurrentHealth) const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	UFUNCTION()
	void OnRep_CurrentRage(const FGameplayAttributeData& OldCurrentRage) const;
	UFUNCTION()
	void OnRep_MaxRage(const FGameplayAttributeData& OldMaxRage) const;
	UFUNCTION()
	void OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower) const;
	UFUNCTION()
	void OnRep_DefensePower(const FGameplayAttributeData& OldDefensePower) const;
};
