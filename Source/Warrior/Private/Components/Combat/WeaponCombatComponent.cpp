// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/WeaponCombatComponent.h"

#include "WarriorGameplayTags.h"
#include "DataAssets/ComboChain/DataAsset_ComboChainBase.h"

void UWeaponCombatComponent::AssignGrantedComboChains(const TMap<int32, FComboChainRule>& InComboChainRulesMap)
{
	InComboChainRulesMap.GenerateValueArray(GrantedComboChains);
	SetActiveComboChainIndexes(GrantedComboChains.Num());
}

void UWeaponCombatComponent::RemoveGrantedComboChains()
{
	// Reset()类似与Empty()，但是不需要重新分配空间
	GrantedComboChains.Empty();
	ActiveComboChainIndexes.Empty();
	CurrentComboCount = 0;
	GetWorld()->GetTimerManager().ClearTimer(ResetTimer);
}

void UWeaponCombatComponent::ResetCombo()
{
	SetActiveComboChainIndexes(GrantedComboChains.Num());
	CurrentComboCount = 0;
	GetWorld()->GetTimerManager().ClearTimer(ResetTimer);
	UE_LOG(LogTemp, Warning, TEXT("ResetCombo"));
}

void UWeaponCombatComponent::ResetComboTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(ResetTimer);
	GetWorld()->GetTimerManager().SetTimer(
		ResetTimer,
		this,
		&ThisClass::ResetCombo,
		ResetComboTime);
}

int32 UWeaponCombatComponent::GetCurrentComboCount() const
{
	return CurrentComboCount;
}

TArray<FComboChainRule> UWeaponCombatComponent::GetGrantedComboChains() const
{
	return GrantedComboChains;
}

FGameplayTag UWeaponCombatComponent::ProcessCombo(FGameplayTag InAbilityComboParentTag)
{
	// 不符合任何Combo分支的动作，比如HitReact、Move、Equip相关GameplayTag，
	// 其中在GA中已用标签阻止Attack/Equip/Unequip，而Move则不会触发Ability，不必考虑
	// todo: Move可以终止Combo
	if (InAbilityComboParentTag.MatchesTag(WarriorGameplayTags::InputTag_Move))
	{
		ResetCombo();
		return FGameplayTag();
	}

	// 每次都需要提前排除掉所有不符合的ActiveComboChainIndex，避免出现"重1轻4"、"轻1重2"等异常情况
	RemoveMismatchedComboChains(InAbilityComboParentTag);
	
	// 搜索是否存在匹配动作
	for (int32 ActiveIndex : ActiveComboChainIndexes)
	{
		const FComboChainRule& ComboChain = GrantedComboChains[ActiveIndex];
		
		// 存在匹配的下一个动作 
		CurrentComboCount++;
		// 已到达分支的最后一个动作，重置计数
		if (ComboChain.ComboSequence.Num() == CurrentComboCount)
		{
			// todo:应该先播放完动作，再重置Combo，否则会出现Combo 4时先显示Reset Combo,然后Combo为0的情况
			// 考虑先AnimNotify中加入ResetCombo功能
			ResetCombo();
			return ComboChain.ComboSequence[ComboChain.ComboSequence.Num() - 1];
		}
		ResetComboTimer();
		return ComboChain.ComboSequence[CurrentComboCount - 1];
	}

	// Combo当前分支中不存在下一个匹配的动作，开启新Combo分支，重置Combo计数
	ResetCombo();
	RemoveMismatchedComboChains(InAbilityComboParentTag);
	
	for (int32 ActiveIndex : ActiveComboChainIndexes)
	{		
		const FComboChainRule& ComboChain = GrantedComboChains[ActiveIndex];
		// 匹配的第一个动作
		CurrentComboCount++;
		return ComboChain.ComboSequence[0];
	}

	ResetCombo();
	return FGameplayTag();
}

void UWeaponCombatComponent::SetActiveComboChainIndexes(int32 InComboChainsNums)
{
	ActiveComboChainIndexes.SetNum(InComboChainsNums);
	for (int32 i = 0; i < InComboChainsNums; i++)
	{
		ActiveComboChainIndexes[i] = i;
	}
}

void UWeaponCombatComponent::RemoveMismatchedComboChains(const FGameplayTag& InAbilityComboParentTag)
{
	/* 错误示例：不应该在遍历时删除元素
	for (int32 ActiveIndex : ActiveComboChainIndexes)
	{
		const FComboChainRule& ComboChain = GrantedComboChains[ActiveIndex];
		// 搜索是否存在匹配动作
		// @note: 注意使用MatchesTag而不是MatchesTagExact，因为InAbilityComboTag是Tag父类
		if (!ComboChain.ComboSequence.IsValidIndex(CurrentComboCount)
			|| !ComboChain.ComboSequence[CurrentComboCount].MatchesTag(InAbilityComboParentTag))
		{
			ActiveComboChainIndexes.Remove(ActiveIndex); //注意不是RemoveAt，因为Remove会导致索引改变，这里要使用的是Value
		}
	}
	*/

	// RemoveAll可以实现边遍历边删除
	ActiveComboChainIndexes.RemoveAll([this, &InAbilityComboParentTag](int32 ActiveIndex)
	{
		const FComboChainRule& ComboChain = GrantedComboChains[ActiveIndex];
		return !ComboChain.ComboSequence.IsValidIndex(CurrentComboCount)
			|| !ComboChain.ComboSequence[CurrentComboCount].MatchesTag(InAbilityComboParentTag);
	});
}
