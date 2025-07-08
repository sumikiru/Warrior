import * as UE from 'ue';
import {$Nullable, blueprint} from "puerts";

const uclass = UE.Class.Load("/Game/Blueprints/Shared/AnimNotifyState/ANS_ToggleWeaponCollision.ANS_ToggleWeaponCollision_C");
const jsClass = blueprint.tojs<typeof UE.Game.Blueprints.Shared.AnimNotifyState.ANS_ToggleWeaponCollision.ANS_ToggleWeaponCollision_C>(uclass);

interface TS_ANS_ToggleWeaponCollision extends UE.Game.Blueprints.Shared.AnimNotifyState.ANS_ToggleWeaponCollision.ANS_ToggleWeaponCollision_C {}
class TS_ANS_ToggleWeaponCollision implements TS_ANS_ToggleWeaponCollision {
    Received_NotifyBegin(MeshComp: $Nullable<UE.SkeletalMeshComponent>, Animation: $Nullable<UE.AnimSequenceBase>, TotalDuration: number, EventReference: UE.AnimNotifyEventReference): boolean {
        // 注意这里的写法：<UE.Actor>MeshComp?.GetOwner()，因为MeshComp可能为null，因此MeshComp?.GetOwner()为undefined | UE.Actor
        // 此时就需要进行类型断言：加上<UE.Actor>
        UE.WarriorFunctionLibrary.CPP_GetPawnCombatComponentFromActor(<UE.Actor>MeshComp?.GetOwner())
            .ToggleWeaponCollision(true, this.ToggleDamageType);
        return false;
    }

    Received_NotifyEnd(MeshComp: $Nullable<UE.SkeletalMeshComponent>, Animation: $Nullable<UE.AnimSequenceBase>, EventReference: UE.AnimNotifyEventReference): boolean {
        UE.WarriorFunctionLibrary.CPP_GetPawnCombatComponentFromActor(<UE.Actor>MeshComp?.GetOwner())
            .ToggleWeaponCollision(false, this.ToggleDamageType);
        return false;
    }
}

blueprint.mixin(jsClass, TS_ANS_ToggleWeaponCollision);