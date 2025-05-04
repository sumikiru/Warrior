// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/WarriorHeroWeapon.h"
#include "GameplayAbilitySpecHandle.h"
#include "Components/Combat/WeaponCombatComponent.h"
#include "DataAssets/ComboChain/DataAsset_AttackCombo.h"

AWarriorHeroWeapon::AWarriorHeroWeapon()
{
	// 这个不需要SetupAttachment，详细结构见蓝图
	WeaponCombatComponent = CreateDefaultSubobject<UWeaponCombatComponent>(TEXT("WeaponCombatComponent"));
}

void AWarriorHeroWeapon::AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles)
{
	GrantedAbilitySpecHandles = InSpecHandles;
}

void AWarriorHeroWeapon::AssignGrantedComboChains()
{
	if (!AttackComboData || AttackComboData->ComboChainRules.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackComboData is empty and cannot assigned to weapon: %s"), *GetName());
		return;
	}
	WeaponCombatComponent->AssignGrantedComboChains(AttackComboData->ComboChainRules);
}

void AWarriorHeroWeapon::RemoveGrantedComboChains()
{
	if (!AttackComboData || AttackComboData->ComboChainRules.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackComboData is empty and there is no need to remove from weapon: %s"), *GetName());
		return;
	}
	WeaponCombatComponent->RemoveGrantedComboChains();
}

TArray<FGameplayAbilitySpecHandle> AWarriorHeroWeapon::GetGrantedAbilitySpecHandles() const
{
	return GrantedAbilitySpecHandles;
}

UWeaponCombatComponent* AWarriorHeroWeapon::GetWeaponCombatComponent() const
{
	return WeaponCombatComponent;
}

