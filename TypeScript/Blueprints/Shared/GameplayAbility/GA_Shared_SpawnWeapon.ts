import * as UE from 'ue';
import {argv, blueprint} from "puerts";

const uclass = UE.Class.Load("/Game/Blueprints/Shared/GameplayAbility/GA_Shared_SpawnWeapon.GA_Shared_SpawnWeapon_C");   // 注意_C后缀
const jsClass = blueprint.tojs<typeof UE.Game.Blueprints.Shared.GameplayAbility.GA_Shared_SpawnWeapon.GA_Shared_SpawnWeapon_C>(uclass);

interface TS_GA_Shared_SpawnWeapon extends UE.Game.Blueprints.Shared.GameplayAbility.GA_Shared_SpawnWeapon.GA_Shared_SpawnWeapon_C {}
class TS_GA_Shared_SpawnWeapon implements TS_GA_Shared_SpawnWeapon {
    // 事件激活能力(Event ActivateAbility)
    K2_ActivateAbility() {
        // 这里不需要super.K2_ActivateAbility();
        //在FJsEnv启动，调用Start时传入的参数可以通过argv获取。如果是继承ue类方式，这里的argv是空的
        let gameInstance = (argv.getByName("GameInstance") as UE.GameInstance);
        // 生成，相当于蓝图的从类生成Actor
        let SpawnedWeapon : UE.WarriorWeaponBase = UE.GameplayStatics.BeginDeferredActorSpawnFromClass(
            gameInstance,
            this.WeaponClassToSpawn,
            UE.Transform.Identity,
            UE.ESpawnActorCollisionHandlingMethod.AdjustIfPossibleButAlwaysSpawn,
            this.GetAvatarActorFromActorInfo()) as UE.WarriorWeaponBase;    // 强制指定返回值类型为子类
        SpawnedWeapon.Instigator = this.GetAvatarActorFromActorInfo() as UE.Pawn;
        UE.GameplayStatics.FinishSpawningActor(SpawnedWeapon, UE.Transform.Identity);

        if (UE.KismetSystemLibrary.IsValid(SpawnedWeapon)){
            // Attach Actor To Component
            SpawnedWeapon.K2_AttachToComponent(
                this.GetOwningComponentFromActorInfo()/* 蓝图为GetSkeletalMeshComponentFromActorInfo */,
                this.SocketNameToAttachTo,
                UE.EAttachmentRule.SnapToTarget,
                UE.EAttachmentRule.KeepRelative,
                UE.EAttachmentRule.KeepWorld,
                true);
            // Register Pawn Combat Component
            this.GetPawnCombatComponentFromActorInfo().RegisterSpawnedWeapon(
                this.WeaponTagToRegister,
                SpawnedWeapon,
                this.RegisterAsEquippedWeapon);
        }

        this.K2_EndAbility();
    }

}

blueprint.mixin(jsClass, TS_GA_Shared_SpawnWeapon);

/**
 * 关于BeginDeferredActorSpawnFromClass：
 * 开始游戏时会发现输出日志中有多行ImportText (LatentInfo): Missing opening parenthesis: LatentInfo
 * 这是由于SpawnWeapon技能在游戏开始时立即激活，和蓝图不同，ts会在PlayInEditor (PIE)前就尝试执行BeginDeferredActorSpawnFromClass
 * 但是Weapon还未加载完成，ts不断尝试会导致输出多次ImportText (LatentInfo): Missing opening parenthesis: LatentInfo
 * 总而言之，这是PuerTS和蓝图执行时机的差异，这并不影响最终结果（从Display级别日志也可以看出）。可以开启Wait for debugger调试验证
 * 将上述内容改用蓝图实现，则无此问题，但没有区别。参考自https://github.com/Tencent/puerts/issues/1594
 */