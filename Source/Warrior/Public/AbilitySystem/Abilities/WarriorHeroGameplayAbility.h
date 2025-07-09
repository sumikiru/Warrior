// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "WarriorHeroGameplayAbility.generated.h"

class UWeaponCombatComponent;
class AWarriorHeroWeapon;
class UWarriorAbilitySystemComponent;
class UHeroCombatComponent;
class AWarriorHeroController;
class AWarriorHeroCharacter;
/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorHeroGameplayAbility : public UWarriorGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	AWarriorHeroCharacter* GetHeroCharacterFromActorInfo();
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	AWarriorHeroController* GetHeroControllerFromActorInfo();
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	UHeroCombatComponent* GetHeroCombatComponentFromActorInfo();
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability|Weapon")
	AWarriorHeroWeapon* GetCurrentCarriedWeaponFromActorInfo();
	// 获取当前武器的战斗组件
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability|Weapon")
	UWeaponCombatComponent* GetCurrentWeaponCombatComponent();
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	UWarriorAbilitySystemComponent* GetWarriorAbilitySystemComponentFromActorInfo();
	/** Apply Damage */
	// Make Gameplay Effect Spec Handle
	UFUNCTION(BlueprintPure, Category = "WarriorAbility")
	FGameplayEffectSpecHandle MakeHeroDamageEffectSpecHandle(const TSubclassOf<UGameplayEffect>& EffectClass, float InWeaponBaseDamage,
	                                                         FGameplayTag InCurrentAttackTypeTag, int32 InCurrentComboCount);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Warrior|Ability", meta = (Categories = "Player.Combo"))
	FGameplayTag AbilityComboParentTag;

private:
	// TWeakObjectPtr适用于当需要避免循环引用，或访问但不控制对象的生命周期（不拥有对象，仅缓存对象引用）时的情况。
	TWeakObjectPtr<AWarriorHeroCharacter> CachedWarriorHeroCharacter;
	TWeakObjectPtr<AWarriorHeroController> CachedWarriorHeroController;
};
