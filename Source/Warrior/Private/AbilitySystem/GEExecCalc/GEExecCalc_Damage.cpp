// sumikiru All Rights Reserved


#include "AbilitySystem/GEExecCalc/GEExecCalc_Damage.h"

#include "AbilitySystem/WarriorAttributeSet.h"

/** 第二种方法：直接使用宏 */
struct FWarriorDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower);

	FWarriorDamageCapture()
	{
		// false启用实时模式，详见第一种方法中关于bSnapshot的注释
		// 注意区分捕获的属性来源，Source or Target
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, AttackPower, Source, false); // 获取自身攻击力
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, DefensePower, Target, false); // 获取目标防御力
	}
};

// 单例模式(这里为Mayer's Singleton，线程安全+延迟初始化)，确保全局唯一实例的创建与访问
static const FWarriorDamageCapture& GetWarriorDamageCapture()
{
	// 该变量在首次创建时初始化，后续调用中始终返回同一实例，确保整个程序中只有一个实例
	static FWarriorDamageCapture WarriorDamageCapture;
	return WarriorDamageCapture;
}

UGEExecCalc_Damage::UGEExecCalc_Damage()
{
	/** 捕获属性的第一种方法，比较复杂，写起来太慢
	// 寻找AttributeSet中的AttackPower属性
	FProperty* AttackPowerProperty = FindFieldChecked<FProperty>(
		UWarriorAttributeSet::StaticClass(),
		GET_MEMBER_NAME_CHECKED(UWarriorAttributeSet, AttackPower)
	);

	// 第三个参数bSnapshot表示用于控制属性值的捕获时机，直接影响GameplayEffect在应用时如何获取目标或源的属性值
	// 如为true属性值会在GameplayEffect创建时立即捕获，即“拍下快照”。即使后续实际应用效果前属性值发生变化，效果内使用的仍是创建时捕获的旧值。
	// 适用场景：需要锁定某一时刻的属性值（如技能释放瞬间的攻击力），避免后续数值变动干扰计算。
	// 如为false代表实时模式，属性值会在GameplayEffect实际应用到目标时捕获。若属性在效果创建和应用之间发生变化，将使用最新的值。
	// 适用场景：需响应实时变化的属性（如目标当前防御力），确保计算基于最新状态。
	FGameplayEffectAttributeCaptureDefinition AttackPowerCaptureDefinition(
		AttackPowerProperty,
		EGameplayEffectAttributeCaptureSource::Source,
		false 
	);

	RelevantAttributesToCapture.Add(AttackPowerCaptureDefinition);
	*/

	RelevantAttributesToCapture.Add(GetWarriorDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetWarriorDamageCapture().DefensePowerDef);
}
