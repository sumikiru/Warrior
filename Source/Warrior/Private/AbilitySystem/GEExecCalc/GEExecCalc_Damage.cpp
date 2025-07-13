// sumikiru All Rights Reserved


#include "AbilitySystem/GEExecCalc/GEExecCalc_Damage.h"

#include "WarriorDebugHelper.h"
#include "WarriorGameplayTags.h"
#include "AbilitySystem/WarriorAttributeSet.h"

/** 第二种方法：直接使用宏 */
struct FWarriorDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken);

	FWarriorDamageCapture()
	{
		// false启用实时模式，详见第一种方法中关于bSnapshot的注释
		// 注意区分捕获的属性来源，Source or Target
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, AttackPower, Source, false); // 获取自身攻击力
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, DefensePower, Target, false); // 获取目标防御力
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, DamageTaken, Target, false); // 获取目标受到的伤害值
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
	RelevantAttributesToCapture.Add(GetWarriorDamageCapture().DamageTakenDef);
}

void UGEExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                               FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	// 参考MakeGameplayEffectSpecHandle（UWarriorHeroGameplayAbility::MakeHeroDamageEffectSpecHandle）中添加了哪些
	/*EffectSpec.GetContext().GetSourceObject();
	EffectSpec.GetContext().GetAbility();
	EffectSpec.GetContext().GetInstigator();
	EffectSpec.GetContext().GetEffectCauser();*/

	// 配置EvaluateParameters，用于后续AttemptCalculateCapturedAttributeMagnitude
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	// 自身的攻击力
	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetWarriorDamageCapture().AttackPowerDef,
		EvaluateParameters,
		SourceAttackPower
	);
	// Debug::Print(TEXT("SourceAttackPower"), SourceAttackPower);

	/**
	 * 在GameplayEffectSpec中检索伤害相关数据(Retrieve Hero Damage Info)
	 * 详见UWarriorHeroGameplayAbility::MakeHeroDamageEffectSpecHandle，这是检索的前提
	 * 通过EffectSpecHandle.Data->SetSetByCallerMagnitude(...);配置了相关的Set By Caller(本质上是一个TMap)
	 */
	float BaseDamage = 0.f;
	int32 CurrentLightAttackComboCount = 0;
	int32 CurrentHeavyAttackComboCount = 0;
	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Shared_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
		}
		if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Player_SetByCaller_AttackType_Light))
		{
			CurrentLightAttackComboCount = TagMagnitude.Value;
			// Debug::Print(TEXT("CurrentLightAttackComboCount"), CurrentLightAttackComboCount);
		}
		if (TagMagnitude.Key.MatchesTagExact(WarriorGameplayTags::Player_SetByCaller_AttackType_Heavy))
		{
			CurrentHeavyAttackComboCount = TagMagnitude.Value;
			// Debug::Print(TEXT("CurrentHeavyAttackComboCount"), CurrentHeavyAttackComboCount);
		}
	}
	
	// 目标的防御力
	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		GetWarriorDamageCapture().DefensePowerDef,
		EvaluateParameters,
		TargetDefensePower
	);
	// Debug::Print(TEXT("TargetDefensePower"), TargetDefensePower);

	/** 执行自定义伤害计算 */
	if (CurrentLightAttackComboCount != 0)
	{
		// ComboCount从1开始计数，表示是当前连招的第几段。第一段不会有额外伤害百分比加成
		const float DamageIncreasePercentLight = (CurrentLightAttackComboCount - 1) * 0.05f + 1.f;
		BaseDamage *= DamageIncreasePercentLight;
		// Debug::Print(TEXT("ScaledBaseDamageLight"), BaseDamage);
	}
	if (CurrentHeavyAttackComboCount != 0)
	{
		const float DamageIncreasePercentHeavy = (CurrentHeavyAttackComboCount - 1) * 0.15f + 1.f;
		BaseDamage *= DamageIncreasePercentHeavy;
		// Debug::Print(TEXT("ScaledBaseDamageHeavy"), BaseDamage);
	}

	const float FinalDamage = BaseDamage * SourceAttackPower / TargetDefensePower;
	// Debug::Print(TEXT("FinalDamage"), FinalDamage);
	
	// 伤害值大于0（有效伤害值），将FinalDamage作为输出值修改DamageTaken
	// @note 这里使用占位符属性DamageTaken而不是直接修改CurrentHealth
	if (FinalDamage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetWarriorDamageCapture().DamageTakenProperty,
				EGameplayModOp::Override,
				FinalDamage
			)
		);
	}

	/**
	 * 之后依据DamageTaken修改CurrentHealth，在UWarriorAttributeSet::PostGameplayEffectExecute中进行。
	 * 注意在此之前还需要在PreAttributeChange()中Clamp当前健康值。
	 */
}
