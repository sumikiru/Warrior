// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UGameplayEffect;
class UWarriorAbilitySystemComponent;
class UWarriorGameplayAbility;
/**
 * 
 */
UCLASS()
class WARRIOR_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	void ApplyStartUpGameplayEffects(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel);
	/**
	 * 将StartUpData赋予给AbilitySystemComponent
	 * @param InASCToGive 要赋予的AbilitySystemComponent
	 * @param ApplyLevel 游戏难度等级
	 */
	virtual void GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UWarriorGameplayAbility>> ActivateOnGivenAbilities;	//需要赋予后立即激活的技能
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UWarriorGameplayAbility>> ReactiveAbilities;	//反应能力，如HitReact/Death
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UGameplayEffect>> StartUpGameplayEffects;	//初始赋予的GameplayEffect，如Health/Power

	// 缺少设置AbilitySpec.DynamicAbilityTags的部分，且InAbilitiesToGive中元素为UWarriorGameplayAbility固定
	void GrantAbilities(const TArray<TSubclassOf<UWarriorGameplayAbility>>& InAbilitiesToGive,
	                    UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
};
