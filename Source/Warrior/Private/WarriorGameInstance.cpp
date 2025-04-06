// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorGameInstance.h"

void UWarriorGameInstance::OnStart()
{
	Super::OnStart();

	if (bDebugMode)
	{
		GameScript = MakeShared<puerts::FJsEnv>(
			std::make_unique<puerts::DefaultJSModuleLoader>(TEXT("JavaScript")),
			std::make_unique<puerts::FDefaultLogger>(),
			8080);
		if (bWaitForDebugger)
		{
			GameScript->WaitDebugger();
		}
	}
	else
	{
		GameScript = MakeShared<puerts::FJsEnv>();
	}

	TArray<TPair<FString, UObject*>> Arguments;
	Arguments.Add({TEXT("GameInstance"), this});
	GameScript->Start("MainGame", Arguments);
}

void UWarriorGameInstance::Shutdown()
{
	Super::Shutdown();

	GameScript.Reset();
}

void UWarriorGameInstance::CallTS(FString FunctionName, UObject* InObj)
{
	Call.Execute(FunctionName, InObj);
}
