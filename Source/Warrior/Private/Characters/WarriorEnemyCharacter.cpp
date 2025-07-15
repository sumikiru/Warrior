// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/WarriorEnemyCharacter.h"

#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/WarriorAttributeSet.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "Engine/AssetManager.h"
#include "GameFramework/CharacterMovementComponent.h"

AWarriorEnemyCharacter::AWarriorEnemyCharacter()
{
	// Enemy的ASC和AS直接在Character构造函数中实例化
	WarriorAbilitySystemComponent = CreateDefaultSubobject<UWarriorAbilitySystemComponent>(TEXT("WarriorAbilitySystemComponent"));
	WarriorAbilitySystemComponent->SetIsReplicated(true);	// 设置ASC用于在网络上复制
	// 和Player（Mixed）不同，Enemy使用Minimal
	WarriorAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	// AttributeSet本身(可视为ASC的一部分)不直接参与网络复制，由ASC处理并将AS中的数据同步到客户端
	WarriorAttributeSet = CreateDefaultSubobject<UWarriorAttributeSet>(TEXT("WarriorAttributeSet"));
	
	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>(TEXT("EnemyCombatComponent"));
}

void AWarriorEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	WarriorAbilitySystemComponent->InitAbilityActorInfo(this, this);
}

UPawnCombatComponent* AWarriorEnemyCharacter::GetPawnCombatComponent() const
{
	return EnemyCombatComponent;
}

void AWarriorEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 异步加载StartUpData
	InitEnemyStartUpData();
}

void AWarriorEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 100.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;
}

void AWarriorEnemyCharacter::InitEnemyStartUpData()
{
	// 与AWarriorHeroCharacter::PossessedBy()中不同之处在于，这里使用异步加载Asynchronous Loading
	// 异步加载需要借助UAssetManager，而同步加载可以直接使用TSoftObject内置的LoadSynchronous()方法
	if (CharacterStartUpData.IsNull())
	{
		return;
	}

	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		CharacterStartUpData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[this]()
			{
				if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.Get())
				{
					LoadedData->GiveToAbilitySystemComponent(WarriorAbilitySystemComponent);
				}
			})
	);
}
