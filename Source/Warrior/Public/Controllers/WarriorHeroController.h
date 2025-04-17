// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WarriorHeroController.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorHeroController : public APlayerController
{
	GENERATED_BODY()

protected:
	// 对于C++的模板类函数，不会暴露到蓝图中，自然PuerTS也就无法调用，需要自己造或者直接在蓝图中编写
	UFUNCTION(BlueprintPure, Category = "SpecifiedForTS")
	UEnhancedInputLocalPlayerSubsystem* GetHeroEnhancedInputLocalPlayerSubsystem() const;
};
