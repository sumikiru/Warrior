import * as UE from 'ue';
import {$ref, blueprint} from "puerts";
import {TS_AbilityTaskFunctionLibrary} from "../../../../Interfaces/GameplayAbility/AbilityTaskFunctionLibrary";

// 前提是设置为“每个Actor实例化”
const uclass = UE.Class.Load("/Game/Blueprints/PlayerCharacter/GameplayAbility/HeavyAttack/GA_Hero_HeavyAttackMaster.GA_Hero_HeavyAttackMaster_C");
const jsClass = blueprint.tojs<typeof UE.Game.Blueprints.PlayerCharacter.GameplayAbility.HeavyAttack.GA_Hero_HeavyAttackMaster.GA_Hero_HeavyAttackMaster_C>(uclass);

interface TS_GA_Hero_HeavyAttackMaster extends UE.Game.Blueprints.PlayerCharacter.GameplayAbility.HeavyAttack.GA_Hero_HeavyAttackMaster.GA_Hero_HeavyAttackMaster_C {}
class TS_GA_Hero_HeavyAttackMaster extends TS_AbilityTaskFunctionLibrary implements TS_GA_Hero_HeavyAttackMaster {
    K2_ActivateAbility() : void {
        this.GetCurrentWeaponCombatComponent().ResetComboTimer();
        this.TS_WaitForResetComboTimer();
        this.TS_WaitForMeleeHitEvent();
        this.TS_PlayMontageAndWait();
    }

    TS_PlayMontageAndWait() : void {
        // 由于该Ability触发前提是携带对应武器攻击，所以直接通过CurrentWeapon获取
        const ComboTag : UE.GameplayTag = this.GetCurrentWeaponCombatComponent().ProcessCombo(this.AbilityComboParentTag);
        super.TS_Lib_PrintDebugString(this.GetCurrentWeaponCombatComponent().GetCurrentComboCount().toString());
        super.TS_Lib_PlayMontageAndWait_AllSameEvents(
            $ref(this.AttackMontagesMap.Get(ComboTag) ?? null)/* 或者直接先对该map元素进行Nullish判断 */,
            (): void => {
                // 通知WeaponCombatComponent重置Combo计时器
                this.TS_NotifyResetComboTimer();
                this.K2_EndAbility();
            }
        );
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
                this.GetCurrentWeaponCombatComponent().ResetComboTimer();
                super.TS_Lib_PrintDebugString("Hitting: " + Payload.EventTag.TagName);
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
}

blueprint.mixin(jsClass, TS_GA_Hero_HeavyAttackMaster);
