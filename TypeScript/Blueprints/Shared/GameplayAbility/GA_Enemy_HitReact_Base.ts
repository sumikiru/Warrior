import * as UE from 'ue';
import * as AsyncUtils from "../../../FunctionLibrary/AsyncUtils";
import * as AbilityTasks from "../../../FunctionLibrary/AbilityTasks";
import {$ref, blueprint} from "puerts";

const uclass = UE.Class.Load("/Game/Blueprints/Shared/GameplayAbility/GA_Enemy_HitReact_Base.GA_Enemy_HitReact_Base_C");   // 注意_C后缀
const jsClass = blueprint.tojs<typeof UE.Game.Blueprints.Shared.GameplayAbility.GA_Enemy_HitReact_Base.GA_Enemy_HitReact_Base_C>(uclass);

interface TS_GA_Enemy_HitReact_Base extends UE.Game.Blueprints.Shared.GameplayAbility.GA_Enemy_HitReact_Base.GA_Enemy_HitReact_Base_C {}
class TS_GA_Enemy_HitReact_Base implements TS_GA_Enemy_HitReact_Base {
    // 使用K2_ActivateAbilityFromEvent而不是K2_ActivateAbility，因为这里GA由GameplayEvent触发(被动技能且需要条件触发)
    // 详见蓝图 类默认值->触发器->能力触发
    K2_ActivateAbilityFromEvent() : void {
        // 设置朝向
        if (this.FaceAttacker) {
            const CalculatedRotator : UE.Rotator = UE.KismetMathLibrary.FindLookAtRotation(
                this.GetEnemyCharacterFromActorInfo().K2_GetActorLocation(),
                // 这里的CurrentEventData就是蓝图事件节点中的Event Data
                this.CurrentEventData.Instigator.K2_GetActorLocation()
            );
            this.GetEnemyCharacterFromActorInfo().K2_SetActorRotation(
                CalculatedRotator,
                false
            );
        }

        // 是否设置有对应的Hit React Montage
        if (this.HasHitReactMontageToPlay) {
            let RandomIndex : number = UE.KismetMathLibrary.RandomInteger(this.MontagesToPlay.Num());
            AbilityTasks.TS_Lib_PlayMontageAndWait_EndAbility(
                this,
                $ref(this.MontagesToPlay.Get(RandomIndex))
            );
            return;
        }

        // K2_ActivateAbilityFromEvent()不是异步函数，所以不能直接使用await DelayNode(2.0);
        AsyncUtils.TS_Lib_Delay(
            2.0,
            () : void => {
                this.K2_EndAbility();
            }
        );
    }

    K2_OnEndAbility(bWasCancelled: boolean) {

    }
}

blueprint.mixin(jsClass, TS_GA_Enemy_HitReact_Base);

