import * as UE from 'ue';
import {$ref, blueprint} from "puerts";
import {TS_AbilityTaskFunctionLibrary} from "../../../../Interfaces/GameplayAbility/AbilityTaskFunctionLibrary";

const uclass = UE.Class.Load("/Game/Blueprints/PlayerCharacter/GameplayAbility/LightAttack/GA_Hero_LightAttackMaster.GA_Hero_LightAttackMaster_C");
const jsClass = blueprint.tojs<typeof UE.Game.Blueprints.PlayerCharacter.GameplayAbility.LightAttack.GA_Hero_LightAttackMaster.GA_Hero_LightAttackMaster_C>(uclass);

interface TS_GA_Hero_LightAttackMaster extends UE.Game.Blueprints.PlayerCharacter.GameplayAbility.LightAttack.GA_Hero_LightAttackMaster.GA_Hero_LightAttackMaster_C {}
class TS_GA_Hero_LightAttackMaster extends TS_AbilityTaskFunctionLibrary implements TS_GA_Hero_LightAttackMaster {
    K2_ActivateAbility() : void {
        // Play Montage And Wait
        this.TS_PlayMontageAndWait();
    }

    TS_PlayMontageAndWait(): void {
        // 由于该Ability触发前提是携带对应武器攻击，所以直接通过CurrentWeapon获取
        const ComboTag : UE.GameplayTag = this.GetCurrentWeaponCombatComponent().ProcessCombo(this.AbilityComboParentTag);
        super.TS_Lib_PlayMontageAndWait_AllSameEvents(
            $ref(this.AttackMontagesMap.Get(ComboTag) ?? null)/* 或者直接先对该map元素进行Nullish判断 */,
            (): void => {
                this.K2_EndAbility();
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
