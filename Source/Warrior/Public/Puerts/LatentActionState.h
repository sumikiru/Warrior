// sumikiru All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LatentActionState.generated.h"

DECLARE_DYNAMIC_DELEGATE(FLatentActionCallback);

/**
 * 参与puerts中的Latent节点构造 \n
 * https://github.com/chexiongsheng/puerts_unreal_demo/blob/master/Source/puerts_unreal_demo/LatentActionState.h \n
 * 具体使用详见Typescript目录下的AsyncUtil.ts和AsyncTest.ts
 */
UCLASS()
class WARRIOR_API ULatentActionState : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FLatentActionCallback LatentActionCallback;

	UFUNCTION()
	virtual void OnLatentActionCompleted(int32 LinkID);
	UFUNCTION(BlueprintCallable)
	FLatentActionInfo GetLatentActionInfo();
};
