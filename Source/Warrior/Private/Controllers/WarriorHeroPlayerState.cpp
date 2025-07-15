// sumikiru All Rights Reserved


#include "Controllers/WarriorHeroPlayerState.h"

#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/WarriorAttributeSet.h"

AWarriorHeroPlayerState::AWarriorHeroPlayerState()
{
	// Player的ASC和AS在PlayerState构造函数中实例化
	WarriorAbilitySystemComponent = CreateDefaultSubobject<UWarriorAbilitySystemComponent>(TEXT("WarriorAbilitySystemComponent"));
	WarriorAbilitySystemComponent->SetIsReplicated(true);	// 设置ASC用于在网络上复制
	//多人模式中，Mixed适合玩家控制的Actors，Minimal适合AI控制的Actors。单人模式使用Full
	WarriorAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	// AttributeSet本身(可视为ASC的一部分)不直接参与网络复制，由ASC处理并将AS中的数据同步到客户端
	WarriorAttributeSet = CreateDefaultSubobject<UWarriorAttributeSet>(TEXT("WarriorAttributeSet"));

	NetUpdateFrequency = 100.f; // 表示每秒和服务器更新频率，默认为2，使用GAS后可以设置的高一些
}

UAbilitySystemComponent* AWarriorHeroPlayerState::GetAbilitySystemComponent() const
{
	return WarriorAbilitySystemComponent;
}
