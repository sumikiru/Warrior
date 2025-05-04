// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "DataAsset_ComboChainBase.generated.h"

USTRUCT(BlueprintType)
struct FComboChainRule
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Description")
	FText Description;	//当前连招分支的描述
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo Sequence", meta = (Categories = "Player.Combo"))
	TArray<FGameplayTag> ComboSequence;
};
/**
 * 
 */
UCLASS()
class WARRIOR_API UDataAsset_ComboChainBase : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<int32, FComboChainRule> ComboChainRules;	//<ComboChainID, FComboChainRule>
};
