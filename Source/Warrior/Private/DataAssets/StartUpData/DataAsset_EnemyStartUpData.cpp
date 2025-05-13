// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"

#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WarriorEnemyGameplayAbility.h"

void UDataAsset_EnemyStartUpData::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

	GrantEnemyAbilities(EnemyCombatAbilities, InASCToGive, ApplyLevel);
}

void UDataAsset_EnemyStartUpData::GrantEnemyAbilities(const TArray<TSubclassOf<UWarriorEnemyGameplayAbility>>& InAbilitiesToGive, UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	if (!InAbilitiesToGive.IsEmpty())
	{
		for (const TSubclassOf<UWarriorEnemyGameplayAbility>& AbilityClass : InAbilitiesToGive)
		{
			if (!AbilityClass)
			{
				continue;
			}
			FGameplayAbilitySpec AbilitySpec(AbilityClass);
			AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
			AbilitySpec.Level = ApplyLevel;
			//AbilitySpec.DynamicAbilityTags=...
			InASCToGive->GiveAbility(AbilitySpec);
		}
	}
}
