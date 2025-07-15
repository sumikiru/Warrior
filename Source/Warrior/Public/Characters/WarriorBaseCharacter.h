// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/PawnCombatInterface.h"
#include "WarriorBaseCharacter.generated.h"

class UDataAsset_StartUpDataBase;
class UWarriorAttributeSet;
class UWarriorAbilitySystemComponent;

UCLASS()
class WARRIOR_API AWarriorBaseCharacter : public ACharacter, public IAbilitySystemInterface, public IPawnCombatInterface
{
	GENERATED_BODY()

public:
	AWarriorBaseCharacter();

	//~ Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface
	//~ Begin IPawnCombatInterface
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	//~ End IPawnCombatInterface
	
protected:
	/**
	 * 1. 对于AI控制的角色，ASC存在于Pawn中，通常在Pawn的 BeginPlay()方法中完成ASC在服务器端和客户端的初始化 \n
	 * 2. 对于玩家控制的角色，如果ASC存在于Pawn中，通常在Pawn的PossessedBy()方法中完成ASC在服务器端的初始化，
	 * 在PlayerController的AcknowledgePawn()方法中完成ASC在客户端的初始化 \n
	 * 3. 对于玩家控制的角色，如果ASC存在于PlayerState中，通常在Pawn的PossessedBy()方法中完成ASC在服务器端的初始化（这一点与上述相同），
	 * 在Pawn的OnRep_PlayerState()方法中完成ASC在客户端的初始化（这将确保PlayerState在客户端已存在）。
	 */
	virtual void PossessedBy(AController* NewController) override;

	/**
	 * Enemy等非玩家角色直接在Character构造函数中实例化，Player等可能伴随重生保留ASC的角色在PlayerState的构造函数中初始化
	 * @note 不论是Enemy还是Player，在Character类中都需要创建ASC和AS。
	 * 注意PlayerState中还需要创建ASC和AS，因此后面在InitWarriorAbilityActorInfo中初始化Character时，Player从PlayerState中获取ASC和AS
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TObjectPtr<UWarriorAbilitySystemComponent> WarriorAbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TObjectPtr<UWarriorAttributeSet> WarriorAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_StartUpDataBase> CharacterStartUpData;

public:
	FORCEINLINE UWarriorAbilitySystemComponent* GetWarriorAbilitySystemComponent() const { return WarriorAbilitySystemComponent; }
	FORCEINLINE UWarriorAttributeSet* GetWarriorAttributeSet() const { return WarriorAttributeSet; }
};
