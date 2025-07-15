import * as UE from 'ue';
import {$Nullable, blueprint} from "puerts";

const uclass = UE.Class.Load("/Game/Blueprints/GameplayCues/GC_Guardian_DeathSound.GC_Guardian_DeathSound_C");   // 注意_C后缀
const jsClass = blueprint.tojs<typeof UE.Game.Blueprints.GameplayCues.GC_Guardian_DeathSound.GC_Guardian_DeathSound_C>(uclass);

interface TS_GC_Guardian_DeathSound extends UE.Game.Blueprints.GameplayCues.GC_Guardian_DeathSound.GC_Guardian_DeathSound_C {}
class TS_GC_Guardian_DeathSound implements TS_GC_Guardian_DeathSound {
    /**
     * 参考GC_Hero_AxeHit
     */
    OnExecute(MyTarget: $Nullable<UE.Actor>, Parameters: UE.GameplayCueParameters): boolean {
        /**
         * x === null检查非空
         * x === null || x === undefined检查非空且有效，可以简化为x == null，因为null === undefined
         */
        if (MyTarget === null || this.GuardianDeathSound == null) {
            return false;
        }
        UE.GameplayStatics.PlaySoundAtLocation(
            MyTarget, // 注意如果为null会无法播放音效，提示A null object was passed as a world context object to UEngine::GetWorldFromContextObject().
            this.GuardianDeathSound,
            MyTarget!.K2_GetActorLocation(), // 使用!.进行非空断言，表明MyTarget一定不是null/undefined
            new UE.Rotator()
        );
        return true;
        // 该蓝图的类为GameplayCueNotify_Static，不是GameplayCueNotify_Static的派生类，
        // 不使用return super.OnExecute(MyTarget, Parameters);
    }
}

blueprint.mixin(jsClass, TS_GC_Guardian_DeathSound);
