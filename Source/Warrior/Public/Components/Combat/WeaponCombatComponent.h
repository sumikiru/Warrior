// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ItemExtensionComponentBase.h"
#include "WeaponCombatComponent.generated.h"

struct FGameplayTag;
struct FComboChainRule;
/**
 * 
 */
UCLASS()
class WARRIOR_API UWeaponCombatComponent : public UItemExtensionComponentBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat|Combo System")
	void AssignGrantedComboChains(const TMap<int32, FComboChainRule>& InComboChainRulesMap);
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat|Combo System")
	void RemoveGrantedComboChains();
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat|Combo System")
	void ResetCombo();
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat|Combo System")
	void ResetComboTimer();
	
	UFUNCTION(BlueprintPure, Category = "Warrior|Combat|Combo System")
	int32 GetCurrentComboCount() const;
	UFUNCTION(BlueprintPure, Category = "Warrior|Combat|Combo System")
	TArray<FComboChainRule> GetGrantedComboChains() const;

	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat|Combo System")
	FGameplayTag ProcessCombo(FGameplayTag InAbilityComboParentTag);

private:
	void SetActiveComboChainIndexes(int32 InComboChainsNums);
	void RemoveMismatchedComboChains(const FGameplayTag& InAbilityComboParentTag);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Warrior|Combat|Combo System", meta = (AllowPrivateAccess = "true"))
	float ResetComboTime = 3.f;
	int32 CurrentComboCount = 0;
	FTimerHandle ResetTimer;
	TArray<FComboChainRule> GrantedComboChains;
	TArray<int32> ActiveComboChainIndexes;
	bool bComboChainsFinished = false;
};
