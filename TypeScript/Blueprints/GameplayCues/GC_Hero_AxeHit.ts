import * as UE from 'ue';
import {$Nullable, blueprint} from "puerts";

const uclass = UE.Class.Load("/Game/Blueprints/GameplayCues/GC_Hero_AxeHit.GC_Hero_AxeHit_C");   // 注意_C后缀
const jsClass = blueprint.tojs<typeof UE.Game.Blueprints.GameplayCues.GC_Hero_AxeHit.GC_Hero_AxeHit_C>(uclass);

interface TS_GC_Hero_AxeHit extends UE.Game.Blueprints.GameplayCues.GC_Hero_AxeHit.GC_Hero_AxeHit_C {}
class TS_GC_Hero_AxeHit implements TS_GC_Hero_AxeHit {
    /**
     * 使用GameplayCueNotify_Static，充当GC处理器的一个非实例化UObject，适用于一次性的爆发状态
     * 而GameplayCueNotify_Actor，充当GC处理器的一个实例化Actor，由于被实例化，因此可以在必要时保持状态，并每帧Tick/更新，适用于持续性的状态
     *
     * 其他地方调用UE.GameplayCueFunctionLibrary.ExecuteGameplayCueOnActor()时会触发该函数
     */
    OnExecute(MyTarget: $Nullable<UE.Actor>, Parameters: UE.GameplayCueParameters): boolean {
        /**
         * x === null检查非空
         * x === null || x === undefined检查非空且有效，可以简化为x == null，因为null === undefined
         */
        if (MyTarget === null || this.AxeHitSound == null) {
            return false;
        }
        UE.GameplayStatics.PlaySoundAtLocation(
            MyTarget, // 注意如果为null会无法播放音效，提示A null object was passed as a world context object to UEngine::GetWorldFromContextObject().
            this.AxeHitSound,
            MyTarget!.K2_GetActorLocation(), // 使用!.进行非空断言，表明MyTarget一定不是null/undefined
            new UE.Rotator()
        );
        return true;
        // 该蓝图的类为GameplayCueNotify_Static，不是GameplayCueNotify_Static的派生类，
        // 不使用return super.OnExecute(MyTarget, Parameters);
    }
}

blueprint.mixin(jsClass, TS_GC_Hero_AxeHit);
