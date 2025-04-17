// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameplayTagContainer.h"

#include "WarriorStructTypes.generated.h"

class UInputMappingContext;
class UWarriorGameplayAbility;
class UWarriorHeroLinkedAnimLayer;
/**
 * 1.关于generated.h
 * 对于任何包含类/结构体/枚举等等的文件（通过USTRUCT，UCLASS，UENUM等判断）, UE将生成所有反射数据并放入该文件中
 * 必须将该文件作为声明类型的标头文件中的最后一个包含语句，将其包含进去，该"generated.h"文件必须在最后include
 * 2.关于CoreMinimal.h
 * CoreMinimal头文件包含一套来自UE4的核心编程环境的普遍存在类型（包含FString，FName，TArray等），被大多数头文件包含
 */
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
 * 将武器相关的Anim Layer/技能/按键直接保存到WeaponData中，Hero装备武器时生效，卸下时移除
 */
USTRUCT(BlueprintType)
struct FWarriorHeroWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWarriorHeroLinkedAnimLayer> WeaponAnimLayerToLink;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> WeaponInputMappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FWarriorHeroAbilitySet> DefaultWeaponAbilities;
};
