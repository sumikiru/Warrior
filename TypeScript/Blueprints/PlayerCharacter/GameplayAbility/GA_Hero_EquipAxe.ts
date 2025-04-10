import * as UE from 'ue';
import {$InRef, $ref, $unref, blueprint} from "puerts";

const uclass = UE.Class.Load("/Game/Blueprints/PlayerCharacter/GameplayAbility/GA_Hero_EquipAxe.GA_Hero_EquipAxe_C");   // 注意_C后缀
const jsClass = blueprint.tojs<typeof UE.Game.Blueprints.PlayerCharacter.GameplayAbility.GA_Hero_EquipAxe.GA_Hero_EquipAxe_C>(uclass);

interface TS_GA_Hero_EquipAxe extends UE.Game.Blueprints.PlayerCharacter.GameplayAbility.GA_Hero_EquipAxe.GA_Hero_EquipAxe_C {}
class TS_GA_Hero_EquipAxe implements TS_GA_Hero_EquipAxe {
    K2_ActivateAbility() {
        // Play Montage And Wait
        this.TS_PlayerMontageAndWait($ref(this.MontageToPlay));
        this.TS_WaitGameplayEvent($ref(this.EventTag));

        //this.K2_EndAbility();
    }

    TS_PlayerMontageAndWait(MontageToPlay: $InRef<UE.AnimMontage>): void {
        let PlayMontageAbilityTask: UE.AbilityTask_PlayMontageAndWait = UE.AbilityTask_PlayMontageAndWait.CreatePlayMontageAndWaitProxy(
            this,
            "None",
            $unref(MontageToPlay));
        PlayMontageAbilityTask.OnCompleted.Add(() => {
            UE.KismetSystemLibrary.PrintString(this, "Completed", false, true);
            this.K2_EndAbility();
        });
        PlayMontageAbilityTask.OnBlendOut.Add(() => {
            UE.KismetSystemLibrary.PrintString(this, "BlendOut", false, true);
            this.K2_EndAbility();
        });
        PlayMontageAbilityTask.OnInterrupted.Add(() => {
            UE.KismetSystemLibrary.PrintString(this, "Interrupted", false, true);
            this.K2_EndAbility();
        });
        PlayMontageAbilityTask.OnCancelled.Add(() => {
            UE.KismetSystemLibrary.PrintString(this, "Cancelled", false, true);
            this.K2_EndAbility();
        });
        // Called to trigger the actual task once the delegates have been set up
        PlayMontageAbilityTask.ReadyForActivation();
    }

    TS_WaitGameplayEvent(EventTag: $InRef<UE.GameplayTag>): void {
        let WaitGameplayEventTask: UE.AbilityTask_WaitGameplayEvent = UE.AbilityTask_WaitGameplayEvent.WaitGameplayEvent(
            this,
            $unref(EventTag));
        WaitGameplayEventTask.EventReceived.Add(() => {
            // Attach Actor To Component
            this.GetHeroCombatComponentFromActorInfo().GetCharacterCarriedWeaponByTag(this.WeaponAxeTag)
                .K2_AttachToComponent(
                    this.GetOwningComponentFromActorInfo(),
                    this.AxeSocketName,
                    UE.EAttachmentRule.SnapToTarget,
                    UE.EAttachmentRule.KeepRelative,
                    UE.EAttachmentRule.KeepWorld,
                    true);
            // Link Anim Class Layers
            this.GetOwningComponentFromActorInfo().LinkAnimClassLayers(
                this.GetHeroCombatComponentFromActorInfo().GetHeroCarriedWeaponByTag(this.WeaponAxeTag)
                    .HeroWeaponData.WeaponAnimLayerToLink);
            // Set Current Equipped Weapon Tag
            this.GetHeroCombatComponentFromActorInfo().CurrentEquippedWeaponTag = this.WeaponAxeTag;
        })
        // 所有AbilityTask的子类在绑定Delegate后都需要手动准备好激活
        WaitGameplayEventTask.ReadyForActivation();
    }
}

blueprint.mixin(jsClass, TS_GA_Hero_EquipAxe);