import * as UE from 'ue';
import {$Nullable, blueprint} from "puerts";

const uclass = UE.Class.Load("/Game/Blueprints/Shared/AnimNotify/AN_SendGameplayEventToOwner.AN_SendGameplayEventToOwner_C");
const jsClass = blueprint.tojs<typeof UE.Game.Blueprints.Shared.AnimNotify.AN_SendGameplayEventToOwner.AN_SendGameplayEventToOwner_C>(uclass);

interface TS_AN_SendGameplayEventToOwner extends UE.Game.Blueprints.Shared.AnimNotify.AN_SendGameplayEventToOwner.AN_SendGameplayEventToOwner_C {}
class TS_AN_SendGameplayEventToOwner implements TS_AN_SendGameplayEventToOwner {
    Received_Notify(MeshComp: $Nullable<UE.SkeletalMeshComponent>, Animation: $Nullable<UE.AnimSequenceBase>, EventReference: UE.AnimNotifyEventReference): boolean {
        UE.AbilitySystemBlueprintLibrary.SendGameplayEventToActor(
            MeshComp?.GetOwner() ?? null,
            this.EventTag,
            new UE.GameplayEventData());
        return false;
    }
}

blueprint.mixin(jsClass, TS_AN_SendGameplayEventToOwner);

/**
 * 1.可选链操作符（?.）
 * 在引用为空(nullish) (null 或者 undefined) 的情况下不会引起错误，该表达式短路返回值是 undefined。
 * 与函数调用一起使用时，如果给定的函数不存在，则返回 undefined。
 * etc.  const var1 = obj?.getter()?.val;   // 如果遇到 null 或 undefined 就可以⽴即停⽌某些表达式的运⾏，直接返回undefined
 *
 * 2.空值合并运算符（??）
 * 当左侧操作数为 null 或 undefined 时，其返回右侧的操作数，否则返回左侧的操作数。
 * etc.  const name = null ?? 'TestString'; // 返回TestString
 * const age = 0 ?? 18; // 返回 0
 * const age2 = 0 || 18; // 返回 18
 *
 * 分析 MeshComp?.GetOwner() ?? null
 * 如果MeshComp为null, 则MeshComp?.GetOwner()为undefined, 故MeshComp?.GetOwner() ?? null 返回值类型为 null
 * 如果MeshComp不为null, 且MeshComp?.GetOwner()为UE.Actor，则MeshComp?.GetOwner() ?? null 返回值类型为 UE.Actor
 */