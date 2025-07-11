// sumikiru All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEExecCalc_Damage.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API UGEExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	// 在构造函数中捕获属性(capture attributes)
	UGEExecCalc_Damage();

	/**
	 * 每当执行(Execute)拥有的GameplayEffect时调用。基本上可以做任何需要做的事情，包括生成新的修饰符(Modifier)以立即执行。
	 * 该自定义计算类(GameplayEffectExecutionCalculation)构造函数用于捕获属性，之后在Execute_Implementation()执行自定义计算
	 * @note 注意UGameplayEffectExecutionCalculation::Execute()中的注释，子类必须重写，同时注意重写的是自动生成的Execute_Implementation() \n
	 * 同时注意由于重写的是自动生成的Execute_Implementation，所以应该为virtual void Execute_Implementation()形式，
	 * 因此不需要UFUNCTION(blueprintNativeEvent) \n
	 * UGameplayEffectExecutionCalculation::Execute()中的是自动生成前的版本，需要blueprintNativeEvent标记，并且没有virtual，会自动生成
	 */
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                                    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
