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
};
