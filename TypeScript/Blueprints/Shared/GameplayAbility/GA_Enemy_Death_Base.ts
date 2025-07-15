import * as UE from 'ue';
import * as AsyncUtils from "../../../FunctionLibrary/AsyncUtils";
import * as AbilityTasks from "../../../FunctionLibrary/AbilityTasks";
import {$ref, blueprint} from "puerts";

const uclass = UE.Class.Load("/Game/Blueprints/Shared/GameplayAbility/GA_Enemy_Death_Base.GA_Enemy_Death_Base_C");   // 注意_C后缀
const jsClass = blueprint.tojs<typeof UE.Game.Blueprints.Shared.GameplayAbility.GA_Enemy_Death_Base.GA_Enemy_Death_Base_C>(uclass);

interface TS_GA_Enemy_Death_Base extends UE.Game.Blueprints.Shared.GameplayAbility.GA_Enemy_Death_Base.GA_Enemy_Death_Base_C {}
class TS_GA_Enemy_Death_Base implements TS_GA_Enemy_Death_Base {
    /**
     * 1.实例化策略为每次执行实例化
     * 2.能力触发的触发源设置为“已拥有标签已添加”(Owned Tag Added)，对应UWarriorFunctionLibrary::AddGameplayTagToActorIfNone
     * 注意这里不是SendGameplayEventToActor,所以也不需要WaitGameplayEvent(不是利用Event触发，而是检测Actor身上的标签)
     */
    K2_ActivateAbility() {
        AbilityTasks.TS_Lib_PrintDebugString("Dead");

        const RandomIndex = UE.KismetMathLibrary.RandomInteger(this.MontagesToPlay.Num());
        AbilityTasks.TS_Lib_PlayMontageAndWait_EndAbility(
            this,
            $ref(this.MontagesToPlay.Get(RandomIndex))
        );

        // 会触发GameplayCueTag对应GC的OnExecute函数
        UE.GameplayCueFunctionLibrary.ExecuteGameplayCueOnActor(
            this.GetOwningActorFromActorInfo(),
            this.DeathSoundGameplayCueTag,
            new UE.GameplayCueParameters()
        );
    }

    K2_OnEndAbility(bWasCancelled: boolean) {

    }
}

blueprint.mixin(jsClass, TS_GA_Enemy_Death_Base);

