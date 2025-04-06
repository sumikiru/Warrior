// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "JsEnv.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WarriorGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void OnStart() override;
	virtual void Shutdown() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bDebugMode = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bWaitForDebugger = false;

	DECLARE_DYNAMIC_DELEGATE_TwoParams(FCall, FString, FunctionName, UObject*, InObj);
	UPROPERTY()
	FCall Call;
	
	UFUNCTION()
	void CallTS(FString FunctionName, UObject* InObj);

private:
	TSharedPtr<puerts::FJsEnv> GameScript;
};
