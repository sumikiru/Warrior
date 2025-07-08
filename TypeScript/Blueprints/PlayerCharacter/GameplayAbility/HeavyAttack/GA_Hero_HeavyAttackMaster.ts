import * as UE from 'ue';
import {$ref, blueprint} from "puerts";
import {TS_AbilityTaskFunctionLibrary} from "../../../../Interfaces/GameplayAbility/AbilityTaskFunctionLibrary";

// 前提是设置为“每个Actor实例化”
const uclass = UE.Class.Load("/Game/Blueprints/PlayerCharacter/GameplayAbility/HeavyAttack/GA_Hero_HeavyAttackMaster.GA_Hero_HeavyAttackMaster_C");
const jsClass = blueprint.tojs<typeof UE.Game.Blueprints.PlayerCharacter.GameplayAbility.HeavyAttack.GA_Hero_HeavyAttackMaster.GA_Hero_HeavyAttackMaster_C>(uclass);

interface TS_GA_Hero_HeavyAttackMaster extends UE.Game.Blueprints.PlayerCharacter.GameplayAbility.HeavyAttack.GA_Hero_HeavyAttackMaster.GA_Hero_HeavyAttackMaster_C {}
class TS_GA_Hero_HeavyAttackMaster extends TS_AbilityTaskFunctionLibrary implements TS_GA_Hero_HeavyAttackMaster {
    K2_ActivateAbility() : void {
        this.TS_PlayMontageAndWait();
        super.TS_Lib_WaitGameplayEvent(
            $ref(UE.GameplayTag.CPP_RequestGameplayTag("Shared.Event.MeleeHit", true)),
            // 这里的Payload就是蓝图中WaitGameplayEvent节点中的Payload
            (Payload : UE.GameplayEventData): void => {
                super.TS_Lib_PrintDebugString("Hitting: " + Payload.EventTag.TagName);
            }
        );
    }

    TS_PlayMontageAndWait() : void {
        // 由于该Ability触发前提是携带对应武器攻击，所以直接通过CurrentWeapon获取
        const ComboTag : UE.GameplayTag = this.GetCurrentWeaponCombatComponent().ProcessCombo(this.AbilityComboParentTag);
        super.TS_Lib_PlayMontageAndWait_AllSameEvents(
            $ref(this.AttackMontagesMap.Get(ComboTag) ?? null)/* 或者直接先对该map元素进行Nullish判断 */,
            (): void => {
                this.K2_EndAbility();
            }
        );
    }
}

blueprint.mixin(jsClass, TS_GA_Hero_HeavyAttackMaster);
