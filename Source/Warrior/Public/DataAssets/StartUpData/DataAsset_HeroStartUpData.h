// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "DataAsset_HeroStartUpData.generated.h"

USTRUCT(BlueprintType)
struct FWarriorHeroAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWarriorGameplayAbility> AbilityToGrant;

	bool IsValid() const;
};

/**
 * 
 */
UCLASS()
class WARRIOR_API UDataAsset_HeroStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel) override;

protected:
	void GrantAbilitiesWithInputTag(const TArray<FWarriorHeroAbilitySet>& InAbilitySetToGive, UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel);

private:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData", meta = (Categories = "InputTag"))
	TArray<FWarriorHeroAbilitySet> HeroStartUpAbilitySets;	//需要输入按键才能执行的能力
};
