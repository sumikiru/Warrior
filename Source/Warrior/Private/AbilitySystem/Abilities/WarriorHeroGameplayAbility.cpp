// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"

#include "WarriorGameplayTags.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Characters/WarriorHeroCharacter.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Controllers/WarriorHeroController.h"
#include "Items/Weapons/WarriorHeroWeapon.h"

AWarriorHeroCharacter* UWarriorHeroGameplayAbility::GetHeroCharacterFromActorInfo()
{
	if (!CachedWarriorHeroCharacter.IsValid())
	{
		CachedWarriorHeroCharacter = Cast<AWarriorHeroCharacter>(CurrentActorInfo->AvatarActor);
	}

	return CachedWarriorHeroCharacter.IsValid() ? CachedWarriorHeroCharacter.Get() : nullptr;
}

AWarriorHeroController* UWarriorHeroGameplayAbility::GetHeroControllerFromActorInfo()
{
	if (!CachedWarriorHeroController.IsValid())
	{
		CachedWarriorHeroController = Cast<AWarriorHeroController>(CurrentActorInfo->PlayerController);
	}

	return CachedWarriorHeroController.IsValid() ? CachedWarriorHeroController.Get() : nullptr;
}

UHeroCombatComponent* UWarriorHeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
	return GetHeroCharacterFromActorInfo()->GetHeroCombatComponent();
}

AWarriorHeroWeapon* UWarriorHeroGameplayAbility::GetCurrentCarriedWeaponFromActorInfo()
{
	return GetHeroCombatComponentFromActorInfo()->GetHeroCarriedWeaponByTag(GetHeroCombatComponentFromActorInfo()->CurrentEquippedWeaponTag);
}

UWeaponCombatComponent* UWarriorHeroGameplayAbility::GetCurrentWeaponCombatComponent()
{
	return GetCurrentCarriedWeaponFromActorInfo()->GetWeaponCombatComponent();
}

UWarriorAbilitySystemComponent* UWarriorHeroGameplayAbility::GetWarriorAbilitySystemComponentFromActorInfo()
{
	return GetHeroCharacterFromActorInfo()->GetWarriorAbilitySystemComponent();
}

FGameplayEffectSpecHandle UWarriorHeroGameplayAbility::MakeHeroDamageEffectSpecHandle(const TSubclassOf<UGameplayEffect>& EffectClass,
                                                                                      float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag,
                                                                                      int32 InCurrentComboCount)
{
	checkf(EffectClass, TEXT("MakeHeroDamageEffectSpecHandle: Invalid EffectClass"));

	FGameplayEffectContextHandle ContextHandle = GetWarriorAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle EffectSpecHandle = GetWarriorAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
	);

	// 设置武器基础伤害. 关于SetByCaller，详见https://sumikiru.github.io/posts/d0ae354a.html#4-5-9-1-SetByCaller
	EffectSpecHandle.Data->SetSetByCallerMagnitude(
		WarriorGameplayTags::Shared_SetByCaller_BaseDamage,
		InWeaponBaseDamage
	);

	// 设置连击伤害加成的数值
	if (InCurrentAttackTypeTag.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(
			InCurrentAttackTypeTag,
			InCurrentComboCount
		);
	}
	
	return EffectSpecHandle;
}
