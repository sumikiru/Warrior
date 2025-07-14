import * as UE from 'ue';
import * as AbilityTasks from "../../../../FunctionLibrary/AbilityTasks";
import {$InRef, $ref, $unref, blueprint} from "puerts";

const uclass = UE.Class.Load("/Game/Blueprints/PlayerCharacter/GameplayAbility/Equipping/GA_Hero_UnequipAxe.GA_Hero_UnequipAxe_C");   // 注意_C后缀
const jsClass = blueprint.tojs<typeof UE.Game.Blueprints.PlayerCharacter.GameplayAbility.Equipping.GA_Hero_UnequipAxe.GA_Hero_UnequipAxe_C>(uclass);

interface TS_GA_Hero_UnequipAxe extends UE.Game.Blueprints.PlayerCharacter.GameplayAbility.Equipping.GA_Hero_UnequipAxe.GA_Hero_UnequipAxe_C {}
class TS_GA_Hero_UnequipAxe implements TS_GA_Hero_UnequipAxe {
    K2_ActivateAbility() {
        // Play Montage And Wait
        AbilityTasks.TS_Lib_PlayMontageAndWait_EndAbility(this, $ref(this.MontageToPlay));
        // Clear Current Equipped Weapon Tag，注意现在应该在WaitGameplayEvent前执行，否则可能出现动画异常
        // 因为每一帧人物移动等动画会根据Weapon的CurrentEquippedWeaponTag来决定
        this.GetHeroCombatComponentFromActorInfo().CurrentEquippedWeaponTag = new UE.GameplayTag();
        this.TS_WaitGameplayEvent($ref(this.EventTag));
        //this.K2_EndAbility();
    }

    TS_WaitGameplayEvent(EventTag: $InRef<UE.GameplayTag>) : void {
        AbilityTasks.TS_Lib_WaitGameplayEvent(
            this,
            EventTag,
            ()=>{
            // Attach Weapon To AxeBackSocket
            this.GetHeroCombatComponentFromActorInfo().GetHeroCarriedWeaponByTag(this.WeaponAxeTag)
                .K2_AttachToComponent(
                    this.GetOwningComponentFromActorInfo(),
                    this.AxeSocketName,
                    UE.EAttachmentRule.SnapToTarget,
                    UE.EAttachmentRule.KeepRelative,
                    UE.EAttachmentRule.KeepWorld,
                    true);
            // Handle Unequip Weapon
            this.TS_HandleUnequipWeapon(
                $ref(this.GetHeroCombatComponentFromActorInfo()
                    .GetHeroCarriedWeaponByTag(this.WeaponAxeTag)));
        });
    }

    TS_HandleUnequipWeapon(InWeaponToUnequip : $InRef<UE.WarriorHeroWeapon>) : void {
        let CachedWeaponData : UE.WarriorHeroWeaponData = $unref(InWeaponToUnequip).HeroWeaponData;
        /* Sequence: 根据WeaponData中的各个属性进行移除 */
        // 1.Unlink Anim Class Layers
        this.GetOwningComponentFromActorInfo().UnlinkAnimClassLayers(CachedWeaponData.WeaponAnimLayerToLink);
        // 2.Remove Input Mapping Context
        this.GetHeroControllerFromActorInfo().GetHeroEnhancedInputLocalPlayerSubsystem().RemoveMappingContext(
            CachedWeaponData.WeaponInputMappingContext
        );
        // 3.Remove Hero Weapon Abilities
        this.GetWarriorAbilitySystemComponentFromActorInfo().RemoveGrantedHeroWeaponAbilities(
            $ref($unref(InWeaponToUnequip).GetGrantedAbilitySpecHandles())
        );
        // 4.Remove Weapon Combo System
        $unref(InWeaponToUnequip).RemoveGrantedComboChains();
    }
}

blueprint.mixin(jsClass, TS_GA_Hero_UnequipAxe);