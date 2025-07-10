import * as UE from 'ue';
import {$ref, blueprint} from "puerts";
import {TS_AbilityTaskFunctionLibrary} from "../../../../Interfaces/GameplayAbility/AbilityTaskFunctionLibrary";

const uclass = UE.Class.Load("/Game/Blueprints/PlayerCharacter/GameplayAbility/LightAttack/GA_Hero_LightAttackMaster.GA_Hero_LightAttackMaster_C");
const jsClass = blueprint.tojs<typeof UE.Game.Blueprints.PlayerCharacter.GameplayAbility.LightAttack.GA_Hero_LightAttackMaster.GA_Hero_LightAttackMaster_C>(uclass);

interface TS_GA_Hero_LightAttackMaster extends UE.Game.Blueprints.PlayerCharacter.GameplayAbility.LightAttack.GA_Hero_LightAttackMaster.GA_Hero_LightAttackMaster_C {}
class TS_GA_Hero_LightAttackMaster extends TS_AbilityTaskFunctionLibrary implements TS_GA_Hero_LightAttackMaster {
    K2_ActivateAbility() : void {
        // 动作开始时先重置一次
        this.GetCurrentWeaponCombatComponent().ResetComboTimer();
        this.TS_WaitForResetComboTimer();
        // 等待命中事件
        this.TS_WaitForMeleeHitEvent();
        this.TS_PlayMontageAndWait();
    }

    TS_PlayMontageAndWait(): void {
        // 由于该Ability触发前提是携带对应武器攻击，所以直接通过CurrentWeapon获取
        const ComboTag : UE.GameplayTag = this.GetCurrentWeaponCombatComponent().ProcessCombo(this.AbilityComboParentTag);
        // @note: 在ProcessCombo之后CurrentComboComboCount才是正常计数
        super.TS_Lib_PrintDebugString(this.GetCurrentWeaponCombatComponent().GetCurrentComboCount().toString());
        super.TS_Lib_PlayMontageAndWait_AllSameEvents(
            $ref(this.AttackMontagesMap.Get(ComboTag) ?? null)/* 或者直接先对该map元素进行Nullish判断 */,
            (): void => {
                this.TS_NotifyResetComboTimer();
                this.K2_EndAbility();
            }
        );
    }

    /** 每次蒙太奇动画开始前准备 */
    TS_WaitForResetComboTimer() : void {
        super.TS_Lib_PrintDebugString("Get Ready To Reset Combo Timer!!");
        super.TS_Lib_WaitGameplayEvent(
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
        super.TS_Lib_PrintDebugString("Reset!!");
    }

    TS_WaitForMeleeHitEvent() : void {
        super.TS_Lib_WaitGameplayEvent(
            /**
             * 参考WarriorGameplayTags.cpp
             * 这样写是给出一种hardcode方案，除了在蓝图中定义一个GameplayTag变量外，还可以直接调用cpp的方法
             * @note 第一个参数为GameplayTag(注意不是下划线，可参照蓝图)，第二个参数为bool ErrorIfNotFound
             */
            $ref(UE.GameplayTag.CPP_RequestGameplayTag("Shared.Event.MeleeHit", true)),
            // 这里的Payload就是蓝图中WaitGameplayEvent节点中的Payload
            (Payload : UE.GameplayEventData): void => {
                super.TS_Lib_PrintDebugString("Hitting: " + Payload.EventTag.TagName);
            }
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

blueprint.mixin(jsClass, TS_GA_Hero_LightAttackMaster);

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
