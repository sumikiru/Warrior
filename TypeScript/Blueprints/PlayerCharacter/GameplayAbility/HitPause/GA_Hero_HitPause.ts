import * as UE from 'ue';
import * as AsyncUtils from '../../../../FunctionLibrary/AsyncUtils'
import {blueprint} from "puerts";

const uclass = UE.Class.Load("/Game/Blueprints/PlayerCharacter/GameplayAbility/HitPause/GA_Hero_HitPause.GA_Hero_HitPause_C");   // 注意_C后缀
const jsClass = blueprint.tojs<typeof UE.Game.Blueprints.PlayerCharacter.GameplayAbility.HitPause.GA_Hero_HitPause.GA_Hero_HitPause_C>(uclass);

interface TS_GA_Hero_HitPause extends UE.Game.Blueprints.PlayerCharacter.GameplayAbility.HitPause.GA_Hero_HitPause.GA_Hero_HitPause_C {}
class TS_GA_Hero_HitPause implements TS_GA_Hero_HitPause {
    K2_ActivateAbilityFromEvent(EventData: UE.GameplayEventData) {
        UE.GameplayStatics.SetGlobalTimeDilation(this.GetOwningActorFromActorInfo(), this.HitPauseTimeDilation);
        AsyncUtils.TS_Lib_Delay(
            0.02,
            () : void => {
                UE.GameplayStatics.SetGlobalTimeDilation(this.GetOwningActorFromActorInfo(), 1.0);
                this.TS_CameraShake();
                this.K2_EndAbility();
            }
        );
    }

    K2_OnEndAbility(bWasCancelled: boolean) {

    }

    TS_CameraShake() : void {
        this.GetHeroControllerFromActorInfo().ClientStartCameraShake(
            this.CameraShakeClass,
            1.0,
            UE.ECameraShakePlaySpace.CameraLocal
        );
    }
}

blueprint.mixin(jsClass, TS_GA_Hero_HitPause);
