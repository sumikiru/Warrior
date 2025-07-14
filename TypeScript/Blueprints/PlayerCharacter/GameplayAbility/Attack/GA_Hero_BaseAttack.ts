import * as UE from 'ue';
import * as AbilityTasks from "../../../../FunctionLibrary/AbilityTasks";
import {$ref, blueprint} from "puerts";

// 前提是设置为“每个Actor实例化”
const uclass = UE.Class.Load("/Game/Blueprints/PlayerCharacter/GameplayAbility/Attack/GA_Hero_BaseAttack.GA_Hero_BaseAttack_C");
const jsClass = blueprint.tojs<typeof UE.Game.Blueprints.PlayerCharacter.GameplayAbility.Attack.GA_Hero_BaseAttack.GA_Hero_BaseAttack_C>(uclass);

interface TS_GA_Hero_BaseAttack extends UE.Game.Blueprints.PlayerCharacter.GameplayAbility.Attack.GA_Hero_BaseAttack.GA_Hero_BaseAttack_C {}
class TS_GA_Hero_BaseAttack implements TS_GA_Hero_BaseAttack {
    K2_ActivateAbility() : void {
        // 动作开始时先重置一次
        this.GetCurrentWeaponCombatComponent().ResetComboTimer();
        this.TS_WaitForResetComboTimer();
        // 等待命中事件
        this.TS_WaitForMeleeHitEvent();
        this.TS_PlayMontageAndWait();
    }

    /** 用来覆盖蓝图节点 */
    K2_OnEndAbility(bWasCancelled: boolean) {

    }

    TS_PlayMontageAndWait(): void {
        // 由于该Ability触发前提是携带对应武器攻击，所以直接通过CurrentWeapon获取
        const ComboTag : UE.GameplayTag = this.GetCurrentWeaponCombatComponent().ProcessCombo(this.AbilityComboParentTag);
        // @note: 在ProcessCombo之后CurrentComboComboCount才是正常计数
        AbilityTasks.TS_Lib_PlayMontageAndWait_AllSameEvents(
            this,
            $ref(this.AttackMontagesMap.Get(ComboTag) ?? null)/* 或者直接先对该map元素进行Nullish判断 */,
            (): void => {
                this.TS_NotifyResetComboTimer();
                this.K2_EndAbility();
            }
        );
    }

    /** 每次蒙太奇动画开始前准备 */
    TS_WaitForResetComboTimer() : void {
        AbilityTasks.TS_Lib_WaitGameplayEvent(
            this,
            $ref(UE.GameplayTag.CPP_RequestGameplayTag("Player.Combo.ResetComboTimer", true)),
            // 这里的Payload就是蓝图中WaitGameplayEvent节点中的Payload
            (Payload : UE.GameplayEventData): void => {
                this.GetCurrentWeaponCombatComponent().ResetComboTimer();
            }
        );
    }

    /** 每次蒙太奇动画结束时触发 */
    TS_NotifyResetComboTimer() : void {
        let Data : UE.GameplayEventData = new UE.GameplayEventData();
        Data.Instigator = this.GetOwningActorFromActorInfo();
        Data.Target = this.GetOwningActorFromActorInfo();

        UE.AbilitySystemBlueprintLibrary.SendGameplayEventToActor(
            this.GetOwningActorFromActorInfo(), // 该Actor必须具有ASC
            UE.GameplayTag.CPP_RequestGameplayTag("Player.Combo.ResetComboTimer", true),
            Data
        );
    }

    TS_WaitForMeleeHitEvent() : void {
        AbilityTasks.TS_Lib_WaitGameplayEvent(
            this,
            /**
             * 参考WarriorGameplayTags.cpp
             * 这样写是给出一种hardcode方案，除了在蓝图中定义一个GameplayTag变量外，还可以直接调用cpp的方法
             * @note 第一个参数为GameplayTag(注意不是下划线，可参照蓝图)，第二个参数为bool ErrorIfNotFound
             */
            $ref(UE.GameplayTag.CPP_RequestGameplayTag("Shared.Event.MeleeHit", true)),
            // 这里的Payload就是蓝图中WaitGameplayEvent节点中的Payload
            /**
             * @note 这里使用匿名函数内部调用this.TS_ApplyDamage()，而不是直接传入this.TS_ApplyDamage
             * 如果是后者，Log会记录报错：无法找到对应函数，这是因为子类没有重写TS_ApplyDamage()，
             * 子类只能调用super.TS_ApplyDamage()而无法调用this.TS_ApplyDamage()
             */
            (Payload : UE.GameplayEventData) : void => {
                // 会触发GameplayCueTag对应GC的OnExecute函数
                UE.GameplayCueFunctionLibrary.ExecuteGameplayCueOnActor(
                    this.GetOwningActorFromActorInfo(),
                    this.WeaponHitSoundGameplayCueTag,
                    new UE.GameplayCueParameters()
                );
                this.TS_ApplyDamage(Payload);
            }
        );
    }

    TS_ApplyDamage(Payload : UE.GameplayEventData) : void {
        // MakeGameplayEffectSpecHandle
        const WeaponBaseDamage = this.GetHeroCombatComponentFromActorInfo()
            .GetHeroCurrentEquippedWeaponDamageAtLevel(
                this.GetAbilityLevel()
            );
        const CurrentComboCount = this.GetCurrentWeaponCombatComponent().GetCurrentComboCount();

        let DamageGameplayEffectSpecHandle : UE.GameplayEffectSpecHandle = this.MakeHeroDamageEffectSpecHandle(
            this.DamageEffectClass,
            WeaponBaseDamage,
            this.CurrentAttackTypeTag,
            CurrentComboCount
        );

        // ApplyGameplayEffectSpecHandleToTarget
        /**
         * @note 成员函数，应该这样调用，同时注意puerts绑定方式：用的是Method而不是Function
         * this.BP_ApplyGameplayEffectSpecHandleToTarget()蓝图版本更直接，但是TS无法修改枚举成员,修改$ref(IsSuccess)不生效
         * 同时枚举成员比较很麻烦，详见后续备注
         * 可以直接在cpp中新建一个TS_xxx方法，不采用枚举而是直接返回true/false来实现，更加方便
         */
        let ActiveGEHandle : UE.ActiveGameplayEffectHandle = this.CPP_ApplyGameplayEffectSpecHandleToTarget(
            Payload.Target,
            DamageGameplayEffectSpecHandle
        );
        if (!(ActiveGEHandle.bPassedFiltersAndWasExecuted)) { // 即ActiveGEHandle.WasWasSuccessfullyApplied()
            AbilityTasks.TS_Lib_PrintDebugString("Apply GameplayEffect Failed");
            return;
        }

        // Hit React
        //const LocalTargetActor = Payload.Target;
        // 这样可以通过事件触发能力：K2_ActivateAbilityFromEvent()
        UE.AbilitySystemBlueprintLibrary.SendGameplayEventToActor(
            //LocalTargetActor,
            Payload.Target,
            UE.GameplayTag.CPP_RequestGameplayTag("Shared.Event.HitReact", true),
            Payload
        );
    }

    /*TS_PlayMontageAndWait(): void {
        super.TS_Lib_PlayMontageAndWait_AllSameEvents(
            $ref(this.AttackMontagesMap.Get(this.CurrentLightAttackComboCount) ?? null)/!* 或者直接先对该map元素进行Nullish判断 *!/,
            (): void => {
                this.K2_EndAbility();
                // Set Timer By Event
                // 注意toDelegate写法，但由于this不是Actor，会由于无法在世界中找到，导致delegate无法生效
                /!*this.ComboCountResetTimerHandle = UE.KismetSystemLibrary.K2_SetTimerDelegate(
                    toDelegate(this, this.TS_ResetAttackComboCount),/!* 注意不是this.TS_ResetAttackComboCount() *!/
                    0.3,
                    false);*!/
                // 采用蓝图辅助，利用自定义事件BP_ResetComboCount来帮助传递Delegate
                this.BP_SetTimerDelegate(2, false, this.ComboCountResetTimerHandle);
            }
        );
    }*/
}

blueprint.mixin(jsClass, TS_GA_Hero_BaseAttack);

/**
 * 关于Delegate
 * 一种方法是直接在C++中声明，然后在ts中调用
 * 第二种方法详见https://puerts.github.io/docs/puerts/unreal/engine_call_script
 * toDelegate(owner: UE.Object, func: Function)这样可以直接在ts文件中临时创建一个Delegate
 * 注意owner是一个UObject，owner释放后自动释放IsJohn
 *
 * 注意一点：详见https://github.com/Tencent/puerts/issues/1233
 * K2_SetTimerDelegate的前提是delegate绑定到的对象是Actor类型
 */

/**
 * 关于===和枚举
 * @note 是===不是==，=== 操作符要求左右两个操作数的类型和值都严格相等，
 * == 操作符会先将两边隐式转换为数字0/1，然后进行比较
 * 枚举之间是不兼容的：即使两个枚举内的成员完全一样，他们也不被认为是相同的类型。
 * 同一枚举内的不同成员也会认为是不同的类型，因此IsSuccess === UE.EWarriorSuccessType.Successful无效
 * 转换为数字或字符串比较是可行的，但是let IsSuccess = UE.EWarriorSuccessType.Successful;类型固定
 */
/*let test : string = IsSuccess.toString();
let Suc : string = UE.EWarriorSuccessType.Successful.toString();
if (test === Suc) {
    super.TS_Lib_PrintDebugString("Apply GameplayEffect Successfully");
}*/
