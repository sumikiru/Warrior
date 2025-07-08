// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Binding.hpp"
#include "UEDataBinding.hpp"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "Characters/WarriorHeroCharacter.h"

/**
 * 详见https://puerts.github.io/docs/puerts/unreal/template_binding/
 * 用于脚本调用普通C++使用UsingCppType, UCLASS使用UsingUClass, USTRUCT使用UsingUStruct
 * @note 注意还需要对参数（如AActor* InActor）的所属类进行声明，如UsingUClass(AActor);
 * 静态成员方法使用.Function,非静态成员方法使用.Method
 * 在cpp文件中实现这一部分，创建结构体及其变量是为了调用构造函数里面的puerts::DefineClass<YouClass>().Function/Method/Property().Register();
 * @note 如果存在报错：Error C2027 : 使用了未定义类型“puerts::converter::Converter<UPawnCombatComponent *,void>”
 * 说明函数的参数或者返回值的类型是未声明的，需要UsingUClass/UsingUStruct等等，注意缺少头文件也会报错
 */
UsingUClass(UWarriorFunctionLibrary);
UsingUClass(AActor);
UsingUStruct(FGameplayTag);
UsingUClass(UPawnCombatComponent);	// for return type

struct FAutoRegisterForCppWarriorFunctionLibrary
{
	FAutoRegisterForCppWarriorFunctionLibrary()
	{
		puerts::DefineClass<UWarriorFunctionLibrary>()
			.Function("CPP_DoesActorHaveTag",MakeFunction(&UWarriorFunctionLibrary::NativeDoesActorHaveTag))
			.Function("CPP_GetPawnCombatComponentFromActor", MakeFunction(&UWarriorFunctionLibrary::NativeGetPawnCombatComponentFromActor))
			.Register();
	}
};
FAutoRegisterForCppWarriorFunctionLibrary _FAutoRegisterForCppWarriorFunctionLibrary__;


UWarriorAbilitySystemComponent* UWarriorFunctionLibrary::NativeGetWarriorASCFromActor(AActor* InActor)
{
	check(InActor);

	return CastChecked<UWarriorAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, const FGameplayTag& TagToAdd)
{
	UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);
	if (!ASC->HasMatchingGameplayTag(TagToAdd))
	{
		//ASC->AddReplicatedLooseGameplayTag(TagToAdd);
		ASC->AddLooseGameplayTag(TagToAdd);
	}
}

void UWarriorFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, const FGameplayTag& TagToRemove)
{
	UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);
	if (ASC->HasMatchingGameplayTag(TagToRemove))
	{
		//ASC->RemoveReplicatedLooseGameplayTag(TagToRemove);
		ASC->RemoveLooseGameplayTag(TagToRemove);
	}
}

bool UWarriorFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, const FGameplayTag& TagToCheck)
{
	UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);

	return ASC->HasMatchingGameplayTag(TagToCheck);
}

AWarriorHeroCharacter* UWarriorFunctionLibrary::NativeGetWarriorHeroCharacter(const UObject* WorldContextObject)
{
	APlayerController* LocalPC = WorldContextObject->GetWorld()->GetFirstPlayerController();
	// 获取本地玩家控制器
	if (LocalPC && LocalPC->IsLocalController())
	{
		return CastChecked<AWarriorHeroCharacter>(LocalPC->GetCharacter());
	}
	return nullptr;
}

UPawnCombatComponent* UWarriorFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
	check(InActor);
	/*// 在UE中，若要通过Execute_XXX形式调用接口方法，接口函数必须使用BlueprintNativeEvent或BlueprintCallable修饰，并通过UFUNCTION宏声明。
	if (InActor->Implements<UPawnCombatInterface>())
	{
		return IPawnCombatInterface::Execute_GetPawnCombatComponent(InActor);
	}*/
	if (const IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
	{
		return PawnCombatInterface->GetPawnCombatComponent();
	}
	return nullptr;
}

void UWarriorFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, const FGameplayTag& TagToCheck, EWarriorConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EWarriorConfirmType::Yes : EWarriorConfirmType::No;
}

UPawnCombatComponent* UWarriorFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor, EWarriorValidType& OutValidType)
{
	UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);
	OutValidType = CombatComponent ? EWarriorValidType::Valid : EWarriorValidType::Invalid;
	return CombatComponent;
}
