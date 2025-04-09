import * as UE from 'ue';
import {blueprint} from "puerts";

const uclass = UE.Class.Load("/Game/Blueprints/Shared/GameplayAbility/GA_Shared_SpawnWeapon.GA_Shared_SpawnWeapon_C");   // 注意_C后缀
const jsClass = blueprint.tojs<typeof UE.Game.Blueprints.Shared.GameplayAbility.GA_Shared_SpawnWeapon.GA_Shared_SpawnWeapon_C>(uclass);

interface TS_GA_Shared_SpawnWeapon extends UE.Game.Blueprints.Shared.GameplayAbility.GA_Shared_SpawnWeapon.GA_Shared_SpawnWeapon_C {}
class TS_GA_Shared_SpawnWeapon implements TS_GA_Shared_SpawnWeapon {
    // 事件激活能力(Event ActivateAbility)
    K2_ActivateAbility() {
        // 这里不需要super.K2_ActivateAbility();
        UE.KismetSystemLibrary.PrintString(this, "来自TS", true, true, UE.LinearColor.Red, 200);

        // 生成，相当于蓝图的从类生成Actor
        let SpawnedWeapon : UE.Actor = UE.GameplayStatics.BeginDeferredActorSpawnFromClass(
            this,
            this.WeaponClassToSpawn,
            new UE.Transform(),
            UE.ESpawnActorCollisionHandlingMethod.AdjustIfPossibleButAlwaysSpawn,
            this.GetAvatarActorFromActorInfo());
        UE.GameplayStatics.FinishSpawningActor(SpawnedWeapon, new UE.Transform());

        // Attach Actor To Component
        if (UE.KismetSystemLibrary.IsValid(SpawnedWeapon)){
            SpawnedWeapon.K2_AttachToComponent(
                this.GetOwningComponentFromActorInfo()/* 蓝图为GetSkeletalMeshComponentFromActorInfo */,
                this.SocketNameToAttachTo,
                UE.EAttachmentRule.SnapToTarget,
                UE.EAttachmentRule.KeepRelative,
                UE.EAttachmentRule.KeepWorld,
                true);
        }

        this.K2_EndAbility();
    }

}

blueprint.mixin(jsClass, TS_GA_Shared_SpawnWeapon);