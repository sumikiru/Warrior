// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "WarriorHeroGameplayAbility.generated.h"

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
	UFUNCTION(BlueprintPure, Category = "WarriorAbility")
	AWarriorHeroCharacter* GetHeroCharacterFromActorInfo();
	UFUNCTION(BlueprintPure, Category = "WarriorAbility")
	AWarriorHeroController* GetHeroControllerFromActorInfo();
	UFUNCTION(BlueprintPure, Category = "WarriorAbility")
	UHeroCombatComponent* GetHeroCombatComponentFromActorInfo();

private:
	// TWeakObjectPtr适用于当需要避免循环引用，或访问但不控制对象的生命周期（不拥有对象，仅缓存对象引用）时的情况。
	TWeakObjectPtr<AWarriorHeroCharacter> CachedWarriorHeroCharacter;
	TWeakObjectPtr<AWarriorHeroController> CachedWarriorHeroController;
};
