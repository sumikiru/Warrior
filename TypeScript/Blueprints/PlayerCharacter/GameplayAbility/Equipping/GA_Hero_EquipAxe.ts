import * as UE from 'ue';
import {GameplayAbilitySpecHandle, TArray} from 'ue';
import {$InRef, $ref, $unref, blueprint} from "puerts";
import {TS_AbilityTaskFunctionLibrary} from "../../../../Interfaces/GameplayAbility/AbilityTaskFunctionLibrary";

const uclass = UE.Class.Load("/Game/Blueprints/PlayerCharacter/GameplayAbility/Equipping/GA_Hero_EquipAxe.GA_Hero_EquipAxe_C");   // 注意_C后缀
const jsClass = blueprint.tojs<typeof UE.Game.Blueprints.PlayerCharacter.GameplayAbility.Equipping.GA_Hero_EquipAxe.GA_Hero_EquipAxe_C>(uclass);

interface TS_GA_Hero_EquipAxe extends UE.Game.Blueprints.PlayerCharacter.GameplayAbility.Equipping.GA_Hero_EquipAxe.GA_Hero_EquipAxe_C {}
class TS_GA_Hero_EquipAxe extends TS_AbilityTaskFunctionLibrary implements TS_GA_Hero_EquipAxe {
    K2_ActivateAbility() {
        // Play Montage And Wait
        // @note: 这里应该调用super方法，this.TS_Lib_PlayMontageAndWait_EndAbility()会报错找不到方法
        // 这一点和C++不同，注意。
        super.TS_Lib_PlayMontageAndWait_EndAbility($ref(this.MontageToPlay));
        //this.TS_Lib_PlayMontageAndWait_EndAbility($ref(this.MontageToPlay));
        this.TS_WaitGameplayEvent($ref(this.EventTag));

        //this.K2_EndAbility();
    }

    TS_WaitGameplayEvent(EventTag: $InRef<UE.GameplayTag>): void {
        super.TS_Lib_WaitGameplayEvent(EventTag, (): void => {
            // Attach Actor To Component
            this.GetHeroCombatComponentFromActorInfo().GetCharacterCarriedWeaponByTag(this.WeaponAxeTag)
                .K2_AttachToComponent(
                    this.GetOwningComponentFromActorInfo(),
                    this.AxeSocketName,
                    UE.EAttachmentRule.SnapToTarget,
                    UE.EAttachmentRule.KeepRelative,
                    UE.EAttachmentRule.KeepWorld,
                    true);
            // Handle Equip Weapon
            this.TS_HandleEquipWeapon(
                $ref(this.GetHeroCombatComponentFromActorInfo()
                    .GetHeroCarriedWeaponByTag(this.WeaponAxeTag)));
            // Set Current Equipped Weapon Tag
            this.GetHeroCombatComponentFromActorInfo().CurrentEquippedWeaponTag = this.WeaponAxeTag;
        });
    }

    TS_HandleEquipWeapon(InWeaponToEquip : $InRef<UE.WarriorHeroWeapon>) : void {
        let CachedHeroWeaponData : UE.WarriorHeroWeaponData = $unref(InWeaponToEquip).HeroWeaponData;
        // 1.Link Anim Class Layers
        this.GetOwningComponentFromActorInfo().LinkAnimClassLayers(
            CachedHeroWeaponData.WeaponAnimLayerToLink
        );
        // 2.Add Mapping Context
        this.GetHeroControllerFromActorInfo().GetHeroEnhancedInputLocalPlayerSubsystem().AddMappingContext(
            CachedHeroWeaponData.WeaponInputMappingContext,
            1 /* 如果按键相同，Priority高的按键映射会覆盖Priority低的按键映射 */);
        // 3.Grant Hero Weapon Abilities
        let GrantedSpecHandles : TArray<GameplayAbilitySpecHandle> = UE.NewArray(UE.GameplayAbilitySpecHandle);
        this.GetWarriorAbilitySystemComponentFromActorInfo().GrantHeroWeaponAbilities(
            CachedHeroWeaponData.DefaultWeaponAbilities,
            this.GetAbilityLevel(),
            $ref(GrantedSpecHandles));
        $unref(InWeaponToEquip).AssignGrantedAbilitySpecHandles(GrantedSpecHandles);
    }
}

blueprint.mixin(jsClass, TS_GA_Hero_EquipAxe);