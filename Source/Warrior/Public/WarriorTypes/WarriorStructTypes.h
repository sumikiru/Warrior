// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WarriorStructTypes.generated.h"

class UWarriorHeroLinkedAnimLayer;
/**
 * 1.关于generated.h
 * 对于任何包含类/结构体/枚举等等的文件（通过USTRUCT，UCLASS，UENUM等判断）, UE将生成所有反射数据并放入该文件中
 * 必须将该文件作为声明类型的标头文件中的最后一个包含语句，将其包含进去，该"generated.h"文件必须在最后include
 * 2.关于CoreMinimal.h
 * CoreMinimal头文件包含一套来自UE4的核心编程环境的普遍存在类型（包含FString，FName，TArray等），被大多数头文件包含
 */
USTRUCT(BlueprintType)
struct FWarriorHeroWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWarriorHeroLinkedAnimLayer> WeaponAnimLayerToLink;
};
