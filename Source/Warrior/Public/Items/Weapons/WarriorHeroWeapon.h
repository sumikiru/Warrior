// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/WarriorWeaponBase.h"
#include "WarriorTypes/WarriorStructTypes.h"
#include "WarriorHeroWeapon.generated.h"

class UWeaponCombatComponent;
class UDataAsset_AttackCombo;
struct FGameplayAbilitySpecHandle;
/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorHeroWeapon : public AWarriorWeaponBase
{
	GENERATED_BODY()

public:
	AWarriorHeroWeapon();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FWarriorHeroWeaponData HeroWeaponData;

	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles);
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat|Combo System")
	void AssignGrantedComboChains();
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat|Combo System")
	void RemoveGrantedComboChains();
	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() const;
	UFUNCTION(BlueprintPure, Category = "Warrior|Combat|Combo System")
	UWeaponCombatComponent* GetWeaponCombatComponent() const;

private:
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataAsset_AttackCombo> AttackComboData;

#pragma region Components
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UWeaponCombatComponent> WeaponCombatComponent;
#pragma endregion

};
