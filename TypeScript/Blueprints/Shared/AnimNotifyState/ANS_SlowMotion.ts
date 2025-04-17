import * as UE from 'ue';
import {$Nullable, blueprint} from "puerts";

const uclass = UE.Class.Load("/Game/Blueprints/Shared/AnimNotifyState/ANS_SlowMotion.ANS_SlowMotion_C");
const jsClass = blueprint.tojs<typeof UE.Game.Blueprints.Shared.AnimNotifyState.ANS_SlowMotion.ANS_SlowMotion_C>(uclass);

interface TS_ANS_SlowMotion extends UE.Game.Blueprints.Shared.AnimNotifyState.ANS_SlowMotion.ANS_SlowMotion_C {}
class TS_ANS_SlowMotion implements TS_ANS_SlowMotion {
    Received_NotifyBegin(MeshComp: $Nullable<UE.SkeletalMeshComponent>, Animation: $Nullable<UE.AnimSequenceBase>, TotalDuration: number, EventReference: UE.AnimNotifyEventReference): boolean {
        UE.GameplayStatics.SetGlobalTimeDilation(MeshComp, this.TimeDilation);
        return false;
    }

    Received_NotifyEnd(MeshComp: $Nullable<UE.SkeletalMeshComponent>, Animation: $Nullable<UE.AnimSequenceBase>, EventReference: UE.AnimNotifyEventReference): boolean {
        UE.GameplayStatics.SetGlobalTimeDilation(MeshComp, 1.0);
        return false;
    }
}

blueprint.mixin(jsClass, TS_ANS_SlowMotion);
